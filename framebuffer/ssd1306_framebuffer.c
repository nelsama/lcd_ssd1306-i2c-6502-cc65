/**
 * ssd1306_framebuffer.c - Frame buffer para SSD1306
 * 
 * Buffer de 512 bytes para display 128x32.
 * Permite manipulación de píxeles individuales.
 */

#include "ssd1306_framebuffer.h"

#if SSD1306_USE_FRAMEBUFFER

#include "../core/ssd1306_core.h"
#include "../../i2c/i2c.h"

/* ============================================
 * BUFFER (512 bytes para 128x32)
 * ============================================ */
static uint8_t framebuffer[SSD1306_WIDTH * SSD1306_PAGES];

/* ============================================
 * FUNCIONES BÁSICAS
 * ============================================ */

void fb_init(void) {
    uint16_t i;
    for (i = 0; i < sizeof(framebuffer); i++) {
        framebuffer[i] = 0x00;
    }
}

void fb_clear(void) {
    fb_init();
}

void fb_fill(void) {
    uint16_t i;
    for (i = 0; i < sizeof(framebuffer); i++) {
        framebuffer[i] = 0xFF;
    }
}

void fb_flush(void) {
    uint8_t page, col;
    
    for (page = 0; page < SSD1306_PAGES; page++) {
        ssd1306_set_pos(0, page);
        i2c_start(SSD1306_ADDR, I2C_WRITE);
        i2c_write_byte(0x40);  /* Data mode */
        for (col = 0; col < SSD1306_WIDTH; col++) {
            i2c_write_byte(framebuffer[page * SSD1306_WIDTH + col]);
        }
        i2c_stop();
    }
}

void fb_set_pixel(uint8_t x, uint8_t y) {
    uint8_t page, bit;
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
    
    page = y >> 3;          /* y / 8 */
    bit = y & 0x07;         /* y % 8 */
    framebuffer[page * SSD1306_WIDTH + x] |= (1 << bit);
}

void fb_clear_pixel(uint8_t x, uint8_t y) {
    uint8_t page, bit;
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
    
    page = y >> 3;
    bit = y & 0x07;
    framebuffer[page * SSD1306_WIDTH + x] &= ~(1 << bit);
}

void fb_toggle_pixel(uint8_t x, uint8_t y) {
    uint8_t page, bit;
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
    
    page = y >> 3;
    bit = y & 0x07;
    framebuffer[page * SSD1306_WIDTH + x] ^= (1 << bit);
}

uint8_t fb_get_pixel(uint8_t x, uint8_t y) {
    uint8_t page, bit;
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return 0;
    
    page = y >> 3;
    bit = y & 0x07;
    return (framebuffer[page * SSD1306_WIDTH + x] >> bit) & 0x01;
}

uint8_t* fb_get_buffer(void) {
    return framebuffer;
}

/* ============================================
 * LÍNEAS (Bresenham)
 * ============================================ */
#if SSD1306_USE_FB_LINE

/* Valor absoluto */
static int16_t fb_abs(int16_t x) {
    return (x < 0) ? -x : x;
}

void fb_line(int8_t x0, int8_t y0, int8_t x1, int8_t y1) {
    int16_t dx = fb_abs(x1 - x0);
    int16_t dy = -fb_abs(y1 - y0);
    int8_t sx = (x0 < x1) ? 1 : -1;
    int8_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx + dy;
    int16_t e2;
    
    while (1) {
        fb_set_pixel(x0, y0);
        
        if (x0 == x1 && y0 == y1) break;
        
        e2 = err << 1;  /* err * 2 */
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void fb_hline(uint8_t x, uint8_t y, uint8_t width) {
    uint8_t i;
    for (i = 0; i < width; i++) {
        fb_set_pixel(x + i, y);
    }
}

void fb_vline(uint8_t x, uint8_t y, uint8_t height) {
    uint8_t i;
    for (i = 0; i < height; i++) {
        fb_set_pixel(x, y + i);
    }
}

void fb_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    fb_hline(x, y, w);              /* Top */
    fb_hline(x, y + h - 1, w);      /* Bottom */
    fb_vline(x, y, h);              /* Left */
    fb_vline(x + w - 1, y, h);      /* Right */
}

#if SSD1306_USE_FB_FILL
void fb_rect_filled(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    uint8_t i, j;
    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            fb_set_pixel(x + i, y + j);
        }
    }
}
#endif /* SSD1306_USE_FB_FILL */

#endif /* SSD1306_USE_FB_LINE */

/* ============================================
 * CÍRCULOS (Midpoint algorithm)
 * ============================================ */
#if SSD1306_USE_FB_CIRCLE

/* Dibujar los 8 puntos simétricos del círculo */
static void fb_circle_points(int8_t cx, int8_t cy, int8_t x, int8_t y) {
    fb_set_pixel(cx + x, cy + y);
    fb_set_pixel(cx - x, cy + y);
    fb_set_pixel(cx + x, cy - y);
    fb_set_pixel(cx - x, cy - y);
    fb_set_pixel(cx + y, cy + x);
    fb_set_pixel(cx - y, cy + x);
    fb_set_pixel(cx + y, cy - x);
    fb_set_pixel(cx - y, cy - x);
}

void fb_circle(int8_t cx, int8_t cy, uint8_t r) {
    int8_t x = 0;
    int8_t y = r;
    int8_t d = 1 - r;
    
    fb_circle_points(cx, cy, x, y);
    
    while (x < y) {
        if (d < 0) {
            d += (x << 1) + 3;  /* 2*x + 3 */
        } else {
            d += ((x - y) << 1) + 5;  /* 2*(x-y) + 5 */
            y--;
        }
        x++;
        fb_circle_points(cx, cy, x, y);
    }
}

#if SSD1306_USE_FB_FILL
/* Línea horizontal para relleno */
static void fb_hline_fill(int8_t x0, int8_t x1, int8_t y) {
    int8_t x;
    if (x0 > x1) { x = x0; x0 = x1; x1 = x; }
    for (x = x0; x <= x1; x++) {
        fb_set_pixel(x, y);
    }
}

void fb_circle_filled(int8_t cx, int8_t cy, uint8_t r) {
    int8_t x = 0;
    int8_t y = r;
    int8_t d = 1 - r;
    
    fb_hline_fill(cx - y, cx + y, cy);
    
    while (x < y) {
        if (d < 0) {
            d += (x << 1) + 3;
        } else {
            d += ((x - y) << 1) + 5;
            y--;
        }
        x++;
        
        fb_hline_fill(cx - x, cx + x, cy + y);
        fb_hline_fill(cx - x, cx + x, cy - y);
        fb_hline_fill(cx - y, cx + y, cy + x);
        fb_hline_fill(cx - y, cx + y, cy - x);
    }
}
#endif /* SSD1306_USE_FB_FILL */

#endif /* SSD1306_USE_FB_CIRCLE */

/* ============================================
 * GRÁFICAS
 * ============================================ */
#if SSD1306_USE_FB_PLOT

void fb_plot(const uint8_t *data, uint8_t count, uint8_t x_offset) {
    uint8_t i;
    uint8_t y;
    
    for (i = 0; i < count && (x_offset + i) < SSD1306_WIDTH; i++) {
        y = data[i];
        if (y >= SSD1306_HEIGHT) y = SSD1306_HEIGHT - 1;
        fb_set_pixel(x_offset + i, SSD1306_HEIGHT - 1 - y);
    }
}

void fb_plot_scaled(const uint8_t *data, uint8_t count, uint8_t min_val, uint8_t max_val) {
    uint8_t i;
    uint8_t range = max_val - min_val;
    uint8_t y;
    
    if (range == 0) range = 1;
    
    for (i = 0; i < count && i < SSD1306_WIDTH; i++) {
        /* Escalar valor al rango 0-31 */
        if (data[i] <= min_val) {
            y = 0;
        } else if (data[i] >= max_val) {
            y = SSD1306_HEIGHT - 1;
        } else {
            y = ((uint16_t)(data[i] - min_val) * (SSD1306_HEIGHT - 1)) / range;
        }
        fb_set_pixel(i, SSD1306_HEIGHT - 1 - y);
    }
}

/* Buffer circular para scroll plot */
static uint8_t plot_buffer[SSD1306_WIDTH];
static uint8_t plot_index = 0;

void fb_plot_scroll(uint8_t value, uint8_t min_val, uint8_t max_val) {
    uint8_t i, idx;
    uint8_t range = max_val - min_val;
    uint8_t y;
    
    if (range == 0) range = 1;
    
    /* Añadir nuevo valor */
    plot_buffer[plot_index] = value;
    plot_index++;
    if (plot_index >= SSD1306_WIDTH) plot_index = 0;
    
    /* Redibujar toda la gráfica */
    fb_clear();
    for (i = 0; i < SSD1306_WIDTH; i++) {
        idx = (plot_index + i) % SSD1306_WIDTH;
        
        if (plot_buffer[idx] <= min_val) {
            y = 0;
        } else if (plot_buffer[idx] >= max_val) {
            y = SSD1306_HEIGHT - 1;
        } else {
            y = ((uint16_t)(plot_buffer[idx] - min_val) * (SSD1306_HEIGHT - 1)) / range;
        }
        fb_set_pixel(i, SSD1306_HEIGHT - 1 - y);
    }
}

#endif /* SSD1306_USE_FB_PLOT */

/* ============================================
 * SPRITES
 * ============================================ */
#if SSD1306_USE_FB_SPRITE

void fb_sprite(uint8_t x, uint8_t y, const uint8_t *sprite, uint8_t w, uint8_t h) {
    uint8_t col, row;
    uint8_t byte_idx;
    
    for (col = 0; col < w; col++) {
        for (row = 0; row < h; row++) {
            byte_idx = col;  /* Sprite organizado por columnas */
            if (sprite[byte_idx] & (1 << row)) {
                fb_set_pixel(x + col, y + row);
            }
        }
    }
}

void fb_sprite_xor(uint8_t x, uint8_t y, const uint8_t *sprite, uint8_t w, uint8_t h) {
    uint8_t col, row;
    uint8_t byte_idx;
    
    for (col = 0; col < w; col++) {
        for (row = 0; row < h; row++) {
            byte_idx = col;
            if (sprite[byte_idx] & (1 << row)) {
                fb_toggle_pixel(x + col, y + row);
            }
        }
    }
}

void fb_sprite_clear(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    uint8_t col, row;
    
    for (col = 0; col < w; col++) {
        for (row = 0; row < h; row++) {
            fb_clear_pixel(x + col, y + row);
        }
    }
}

#endif /* SSD1306_USE_FB_SPRITE */

#endif /* SSD1306_USE_FRAMEBUFFER */

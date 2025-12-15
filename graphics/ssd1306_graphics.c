/**
 * ssd1306_graphics.c - Funciones gráficas
 */

#include "ssd1306_graphics.h"
#include "../core/ssd1306_core.h"
#include "../../i2c/i2c.h"

#if SSD1306_USE_GRAPHICS

/* Línea horizontal */
void ssd1306_hline(uint8_t x, uint8_t page, uint8_t width, uint8_t pattern) {
    uint8_t i;
    
    ssd1306_set_pos(x, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    for (i = 0; i < width; i++) {
        i2c_write_byte(pattern);
    }
    i2c_stop();
}

/* Llenar página con patrón */
void ssd1306_fill_page(uint8_t page, uint8_t pattern) {
    uint8_t col;
    
    ssd1306_set_pos(0, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    for (col = 0; col < SSD1306_WIDTH; col++) {
        i2c_write_byte(pattern);
    }
    i2c_stop();
}

#endif /* SSD1306_USE_GRAPHICS */

#if SSD1306_USE_RECT

/* Rectángulo */
void ssd1306_rect(uint8_t x, uint8_t page, uint8_t width, uint8_t height_pages) {
    uint8_t i, p;
    
    for (p = 0; p < height_pages; p++) {
        ssd1306_set_pos(x, page + p);
        
        i2c_start(SSD1306_ADDR, I2C_WRITE);
        i2c_write_byte(SSD1306_DATA);
        
        if (p == 0) {
            /* Borde superior */
            for (i = 0; i < width; i++) {
                if (i == 0 || i == width - 1) {
                    i2c_write_byte(0xFF);
                } else {
                    i2c_write_byte(0x01);
                }
            }
        } else if (p == height_pages - 1) {
            /* Borde inferior */
            for (i = 0; i < width; i++) {
                if (i == 0 || i == width - 1) {
                    i2c_write_byte(0xFF);
                } else {
                    i2c_write_byte(0x80);
                }
            }
        } else {
            /* Lados */
            i2c_write_byte(0xFF);
            for (i = 1; i < width - 1; i++) {
                i2c_write_byte(0x00);
            }
            i2c_write_byte(0xFF);
        }
        i2c_stop();
    }
}

#endif /* SSD1306_USE_RECT */

#if SSD1306_USE_PROGRESS

/* Barra de progreso */
void ssd1306_progress_bar(uint8_t x, uint8_t page, uint8_t width, uint8_t percent) {
    uint8_t i;
    uint8_t filled;
    
    if (percent > 100) percent = 100;
    filled = (uint8_t)(((uint16_t)width * percent) / 100);
    
    ssd1306_set_pos(x, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    
    for (i = 0; i < width; i++) {
        if (i == 0) {
            i2c_write_byte(0x7E);  /* Borde izq */
        } else if (i == width - 1) {
            i2c_write_byte(0x7E);  /* Borde der */
        } else if (i <= filled) {
            i2c_write_byte(0x7E);  /* Lleno */
        } else {
            i2c_write_byte(0x42);  /* Vacío */
        }
    }
    i2c_stop();
}

#endif /* SSD1306_USE_PROGRESS */

#if SSD1306_USE_ICONS

/* Dibujar icono 8x8 */
void ssd1306_icon8(uint8_t col, uint8_t page, const uint8_t *icon) {
    uint8_t i;
    
    ssd1306_set_pos(col, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    for (i = 0; i < 8; i++) {
        i2c_write_byte(icon[i]);
    }
    i2c_stop();
}

/* Limpiar área */
void ssd1306_clear_area(uint8_t col, uint8_t page, uint8_t width) {
    uint8_t i;
    
    ssd1306_set_pos(col, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    for (i = 0; i < width; i++) {
        i2c_write_byte(0x00);
    }
    i2c_stop();
}

#endif /* SSD1306_USE_ICONS */

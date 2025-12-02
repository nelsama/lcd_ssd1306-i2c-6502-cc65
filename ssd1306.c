/**
 * ssd1306.c - Driver completo SSD1306 para 6502
 * Sin framebuffer - escribe directo al display
 * Soporta: texto, números, gráficos básicos, scroll, inversión
 */

#include "ssd1306.h"
#include "../i2c/i2c.h"

/* Comandos SSD1306 */
#define SSD1306_CMD         0x00
#define SSD1306_DATA        0x40

/* Fuente 5x7 completa - caracteres 32-127 (ASCII imprimibles) */
static const uint8_t font5x7[] = {
    /* 32-47: Espacio y símbolos */
    0x00, 0x00, 0x00, 0x00, 0x00, /* space */
    0x00, 0x00, 0x5F, 0x00, 0x00, /* ! */
    0x00, 0x07, 0x00, 0x07, 0x00, /* " */
    0x14, 0x7F, 0x14, 0x7F, 0x14, /* # */
    0x24, 0x2A, 0x7F, 0x2A, 0x12, /* $ */
    0x23, 0x13, 0x08, 0x64, 0x62, /* % */
    0x36, 0x49, 0x55, 0x22, 0x50, /* & */
    0x00, 0x05, 0x03, 0x00, 0x00, /* ' */
    0x00, 0x1C, 0x22, 0x41, 0x00, /* ( */
    0x00, 0x41, 0x22, 0x1C, 0x00, /* ) */
    0x08, 0x2A, 0x1C, 0x2A, 0x08, /* * */
    0x08, 0x08, 0x3E, 0x08, 0x08, /* + */
    0x00, 0x50, 0x30, 0x00, 0x00, /* , */
    0x08, 0x08, 0x08, 0x08, 0x08, /* - */
    0x00, 0x60, 0x60, 0x00, 0x00, /* . */
    0x20, 0x10, 0x08, 0x04, 0x02, /* / */
    /* 48-57: Números 0-9 */
    0x3E, 0x51, 0x49, 0x45, 0x3E, /* 0 */
    0x00, 0x42, 0x7F, 0x40, 0x00, /* 1 */
    0x42, 0x61, 0x51, 0x49, 0x46, /* 2 */
    0x21, 0x41, 0x45, 0x4B, 0x31, /* 3 */
    0x18, 0x14, 0x12, 0x7F, 0x10, /* 4 */
    0x27, 0x45, 0x45, 0x45, 0x39, /* 5 */
    0x3C, 0x4A, 0x49, 0x49, 0x30, /* 6 */
    0x01, 0x71, 0x09, 0x05, 0x03, /* 7 */
    0x36, 0x49, 0x49, 0x49, 0x36, /* 8 */
    0x06, 0x49, 0x49, 0x29, 0x1E, /* 9 */
    /* 58-64: Símbolos */
    0x00, 0x36, 0x36, 0x00, 0x00, /* : */
    0x00, 0x56, 0x36, 0x00, 0x00, /* ; */
    0x00, 0x08, 0x14, 0x22, 0x41, /* < */
    0x14, 0x14, 0x14, 0x14, 0x14, /* = */
    0x41, 0x22, 0x14, 0x08, 0x00, /* > */
    0x02, 0x01, 0x51, 0x09, 0x06, /* ? */
    0x32, 0x49, 0x79, 0x41, 0x3E, /* @ */
    /* 65-90: Mayúsculas A-Z */
    0x7E, 0x11, 0x11, 0x11, 0x7E, /* A */
    0x7F, 0x49, 0x49, 0x49, 0x36, /* B */
    0x3E, 0x41, 0x41, 0x41, 0x22, /* C */
    0x7F, 0x41, 0x41, 0x22, 0x1C, /* D */
    0x7F, 0x49, 0x49, 0x49, 0x41, /* E */
    0x7F, 0x09, 0x09, 0x01, 0x01, /* F */
    0x3E, 0x41, 0x41, 0x51, 0x32, /* G */
    0x7F, 0x08, 0x08, 0x08, 0x7F, /* H */
    0x00, 0x41, 0x7F, 0x41, 0x00, /* I */
    0x20, 0x40, 0x41, 0x3F, 0x01, /* J */
    0x7F, 0x08, 0x14, 0x22, 0x41, /* K */
    0x7F, 0x40, 0x40, 0x40, 0x40, /* L */
    0x7F, 0x02, 0x04, 0x02, 0x7F, /* M */
    0x7F, 0x04, 0x08, 0x10, 0x7F, /* N */
    0x3E, 0x41, 0x41, 0x41, 0x3E, /* O */
    0x7F, 0x09, 0x09, 0x09, 0x06, /* P */
    0x3E, 0x41, 0x51, 0x21, 0x5E, /* Q */
    0x7F, 0x09, 0x19, 0x29, 0x46, /* R */
    0x46, 0x49, 0x49, 0x49, 0x31, /* S */
    0x01, 0x01, 0x7F, 0x01, 0x01, /* T */
    0x3F, 0x40, 0x40, 0x40, 0x3F, /* U */
    0x1F, 0x20, 0x40, 0x20, 0x1F, /* V */
    0x7F, 0x20, 0x18, 0x20, 0x7F, /* W */
    0x63, 0x14, 0x08, 0x14, 0x63, /* X */
    0x03, 0x04, 0x78, 0x04, 0x03, /* Y */
    0x61, 0x51, 0x49, 0x45, 0x43, /* Z */
    /* 91-96: Símbolos */
    0x00, 0x00, 0x7F, 0x41, 0x41, /* [ */
    0x02, 0x04, 0x08, 0x10, 0x20, /* \ */
    0x41, 0x41, 0x7F, 0x00, 0x00, /* ] */
    0x04, 0x02, 0x01, 0x02, 0x04, /* ^ */
    0x40, 0x40, 0x40, 0x40, 0x40, /* _ */
    0x00, 0x01, 0x02, 0x04, 0x00, /* ` */
    /* 97-122: Minúsculas a-z */
    0x20, 0x54, 0x54, 0x54, 0x78, /* a */
    0x7F, 0x48, 0x44, 0x44, 0x38, /* b */
    0x38, 0x44, 0x44, 0x44, 0x20, /* c */
    0x38, 0x44, 0x44, 0x48, 0x7F, /* d */
    0x38, 0x54, 0x54, 0x54, 0x18, /* e */
    0x08, 0x7E, 0x09, 0x01, 0x02, /* f */
    0x08, 0x54, 0x54, 0x54, 0x3C, /* g */
    0x7F, 0x08, 0x04, 0x04, 0x78, /* h */
    0x00, 0x44, 0x7D, 0x40, 0x00, /* i */
    0x20, 0x40, 0x44, 0x3D, 0x00, /* j */
    0x00, 0x7F, 0x10, 0x28, 0x44, /* k */
    0x00, 0x41, 0x7F, 0x40, 0x00, /* l */
    0x7C, 0x04, 0x18, 0x04, 0x78, /* m */
    0x7C, 0x08, 0x04, 0x04, 0x78, /* n */
    0x38, 0x44, 0x44, 0x44, 0x38, /* o */
    0x7C, 0x14, 0x14, 0x14, 0x08, /* p */
    0x08, 0x14, 0x14, 0x18, 0x7C, /* q */
    0x7C, 0x08, 0x04, 0x04, 0x08, /* r */
    0x48, 0x54, 0x54, 0x54, 0x20, /* s */
    0x04, 0x3F, 0x44, 0x40, 0x20, /* t */
    0x3C, 0x40, 0x40, 0x20, 0x7C, /* u */
    0x1C, 0x20, 0x40, 0x20, 0x1C, /* v */
    0x3C, 0x40, 0x30, 0x40, 0x3C, /* w */
    0x44, 0x28, 0x10, 0x28, 0x44, /* x */
    0x0C, 0x50, 0x50, 0x50, 0x3C, /* y */
    0x44, 0x64, 0x54, 0x4C, 0x44, /* z */
    /* 123-126: Símbolos finales */
    0x00, 0x08, 0x36, 0x41, 0x00, /* { */
    0x00, 0x00, 0x7F, 0x00, 0x00, /* | */
    0x00, 0x41, 0x36, 0x08, 0x00, /* } */
    0x08, 0x08, 0x2A, 0x1C, 0x08, /* -> */
};

/* ============================================
 * FUNCIONES INTERNAS
 * ============================================ */

/* Enviar comando */
static void ssd1306_cmd(uint8_t cmd) {
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_CMD);
    i2c_write_byte(cmd);
    i2c_stop();
}

/* Posicionar cursor */
static void ssd1306_set_pos(uint8_t col, uint8_t page) {
    ssd1306_cmd(0xB0 + page);
    ssd1306_cmd(0x00 + (col & 0x0F));
    ssd1306_cmd(0x10 + ((col >> 4) & 0x0F));
}

/* ============================================
 * FUNCIONES BÁSICAS
 * ============================================ */

/* Inicializar display 128x32 */
uint8_t ssd1306_init(void) {
    /* Verificar que responde */
    if (!i2c_start(SSD1306_ADDR, I2C_WRITE)) {
        i2c_stop();
        return 0;
    }
    i2c_stop();
    
    /* Secuencia de inicialización para 128x32 */
    ssd1306_cmd(0xAE); /* Display off */
    ssd1306_cmd(0xD5); /* Set clock div */
    ssd1306_cmd(0x80);
    ssd1306_cmd(0xA8); /* Set multiplex */
    ssd1306_cmd(0x1F); /* 32 líneas */
    ssd1306_cmd(0xD3); /* Set display offset */
    ssd1306_cmd(0x00);
    ssd1306_cmd(0x40); /* Set start line */
    ssd1306_cmd(0x8D); /* Charge pump */
    ssd1306_cmd(0x14);
    ssd1306_cmd(0x20); /* Memory mode */
    ssd1306_cmd(0x00); /* Horizontal */
    ssd1306_cmd(0xA1); /* Seg remap */
    ssd1306_cmd(0xC8); /* COM scan dec */
    ssd1306_cmd(0xDA); /* Set COM pins */
    ssd1306_cmd(0x02);
    ssd1306_cmd(0x81); /* Set contrast */
    ssd1306_cmd(0x8F);
    ssd1306_cmd(0xD9); /* Set precharge */
    ssd1306_cmd(0xF1);
    ssd1306_cmd(0xDB); /* Set VCOMH */
    ssd1306_cmd(0x40);
    ssd1306_cmd(0xA4); /* Display RAM */
    ssd1306_cmd(0xA6); /* Normal display */
    ssd1306_cmd(0xAF); /* Display on */
    
    return 1;
}

/* Limpiar pantalla */
void ssd1306_clear(void) {
    uint8_t page, col;
    
    for (page = 0; page < 4; page++) {
        ssd1306_set_pos(0, page);
        
        i2c_start(SSD1306_ADDR, I2C_WRITE);
        i2c_write_byte(SSD1306_DATA);
        for (col = 0; col < 128; col++) {
            i2c_write_byte(0x00);
        }
        i2c_stop();
    }
}

/* Encender display */
void ssd1306_display_on(void) {
    ssd1306_cmd(0xAF);
}

/* Apagar display */
void ssd1306_display_off(void) {
    ssd1306_cmd(0xAE);
}

/* ============================================
 * CONTROL DE DISPLAY
 * ============================================ */

/* Ajustar contraste */
void ssd1306_set_contrast(uint8_t contrast) {
    ssd1306_cmd(0x81);
    ssd1306_cmd(contrast);
}

/* Invertir colores */
void ssd1306_invert(uint8_t invert) {
    ssd1306_cmd(invert ? 0xA7 : 0xA6);
}

/* Flip horizontal */
void ssd1306_flip_h(uint8_t flip) {
    ssd1306_cmd(flip ? 0xA0 : 0xA1);
}

/* Flip vertical */
void ssd1306_flip_v(uint8_t flip) {
    ssd1306_cmd(flip ? 0xC0 : 0xC8);
}

/* Scroll derecha */
void ssd1306_scroll_right(uint8_t start_page, uint8_t end_page, uint8_t speed) {
    ssd1306_cmd(0x26);        /* Right scroll */
    ssd1306_cmd(0x00);        /* Dummy */
    ssd1306_cmd(start_page);  /* Start page */
    ssd1306_cmd(speed);       /* Speed (0-7) */
    ssd1306_cmd(end_page);    /* End page */
    ssd1306_cmd(0x00);        /* Dummy */
    ssd1306_cmd(0xFF);        /* Dummy */
    ssd1306_cmd(0x2F);        /* Activate scroll */
}

/* Scroll izquierda */
void ssd1306_scroll_left(uint8_t start_page, uint8_t end_page, uint8_t speed) {
    ssd1306_cmd(0x27);        /* Left scroll */
    ssd1306_cmd(0x00);        /* Dummy */
    ssd1306_cmd(start_page);  /* Start page */
    ssd1306_cmd(speed);       /* Speed (0-7) */
    ssd1306_cmd(end_page);    /* End page */
    ssd1306_cmd(0x00);        /* Dummy */
    ssd1306_cmd(0xFF);        /* Dummy */
    ssd1306_cmd(0x2F);        /* Activate scroll */
}

/* Detener scroll */
void ssd1306_scroll_stop(void) {
    ssd1306_cmd(0x2E);
}

/* ============================================
 * TEXTO
 * ============================================ */

/* Escribir un caracter */
void ssd1306_char(uint8_t col, uint8_t page, char c) {
    uint8_t i;
    const uint8_t *glyph;
    
    ssd1306_set_pos(col, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    
    if (c >= 32 && c <= 126) {
        glyph = &font5x7[(c - 32) * 5];
        for (i = 0; i < 5; i++) {
            i2c_write_byte(glyph[i]);
        }
        i2c_write_byte(0x00);
    }
    
    i2c_stop();
}

/* Escribir texto */
void ssd1306_text(uint8_t col, uint8_t page, const char *str) {
    uint8_t c, i;
    const uint8_t *glyph;
    
    ssd1306_set_pos(col, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    
    while (*str) {
        c = *str;
        if (c >= 32 && c <= 126) {
            glyph = &font5x7[(c - 32) * 5];
            for (i = 0; i < 5; i++) {
                i2c_write_byte(glyph[i]);
            }
            i2c_write_byte(0x00);
        }
        str++;
    }
    
    i2c_stop();
}

/* Escribir texto invertido */
void ssd1306_text_inv(uint8_t col, uint8_t page, const char *str) {
    uint8_t c, i;
    const uint8_t *glyph;
    
    ssd1306_set_pos(col, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    
    while (*str) {
        c = *str;
        if (c >= 32 && c <= 126) {
            glyph = &font5x7[(c - 32) * 5];
            for (i = 0; i < 5; i++) {
                i2c_write_byte(glyph[i] ^ 0xFF);
            }
            i2c_write_byte(0xFF);
        }
        str++;
    }
    
    i2c_stop();
}

/* ============================================
 * NÚMEROS
 * ============================================ */

/* Mostrar número decimal */
void ssd1306_number(uint8_t col, uint8_t page, uint16_t num) {
    char buf[6];
    uint8_t i = 5;
    
    buf[5] = '\0';
    do {
        i--;
        buf[i] = '0' + (num % 10);
        num /= 10;
    } while (num > 0 && i > 0);
    
    ssd1306_text(col, page, &buf[i]);
}

/* Mostrar número con signo */
void ssd1306_number_signed(uint8_t col, uint8_t page, int16_t num) {
    if (num < 0) {
        ssd1306_char(col, page, '-');
        ssd1306_number(col + 6, page, (uint16_t)(-num));
    } else {
        ssd1306_number(col, page, (uint16_t)num);
    }
}

/* Mostrar hex 8 bits */
void ssd1306_hex8(uint8_t col, uint8_t page, uint8_t num) {
    static const char hex[] = "0123456789ABCDEF";
    char buf[3];
    
    buf[0] = hex[num >> 4];
    buf[1] = hex[num & 0x0F];
    buf[2] = '\0';
    
    ssd1306_text(col, page, buf);
}

/* Mostrar hex 16 bits */
void ssd1306_hex16(uint8_t col, uint8_t page, uint16_t num) {
    ssd1306_hex8(col, page, num >> 8);
    ssd1306_hex8(col + 12, page, num & 0xFF);
}

/* ============================================
 * GRÁFICOS BÁSICOS
 * ============================================ */

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
    for (col = 0; col < 128; col++) {
        i2c_write_byte(pattern);
    }
    i2c_stop();
}

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

/* ============================================
 * ICONOS Y SÍMBOLOS
 * ============================================ */

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

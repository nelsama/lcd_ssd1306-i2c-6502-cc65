/**
 * ssd1306_text.c - Funciones de texto
 */

#include "ssd1306_text.h"
#include "../core/ssd1306_core.h"
#include "../fonts/ssd1306_font.h"
#include "../../i2c/i2c.h"

#if SSD1306_USE_TEXT

/* Escribir un caracter */
void ssd1306_char(uint8_t col, uint8_t page, char c) {
    uint8_t i;
    const uint8_t *glyph;
    uint8_t width = font_get_width();
    
    ssd1306_set_pos(col, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    
    glyph = font_get_glyph(c);
    for (i = 0; i < width; i++) {
        i2c_write_byte(glyph[i]);
    }
    i2c_write_byte(0x00); /* Espacio entre caracteres */
    
    i2c_stop();
}

/* Escribir texto */
void ssd1306_text(uint8_t col, uint8_t page, const char *str) {
    uint8_t i;
    const uint8_t *glyph;
    uint8_t width = font_get_width();
    
    ssd1306_set_pos(col, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    
    while (*str) {
        glyph = font_get_glyph(*str);
        for (i = 0; i < width; i++) {
            i2c_write_byte(glyph[i]);
        }
        i2c_write_byte(0x00); /* Espacio */
        str++;
    }
    
    i2c_stop();
}

#if SSD1306_USE_TEXT_INV
/* Escribir texto invertido */
void ssd1306_text_inv(uint8_t col, uint8_t page, const char *str) {
    uint8_t i;
    const uint8_t *glyph;
    uint8_t width = font_get_width();
    
    ssd1306_set_pos(col, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    
    while (*str) {
        glyph = font_get_glyph(*str);
        for (i = 0; i < width; i++) {
            i2c_write_byte(glyph[i] ^ 0xFF);
        }
        i2c_write_byte(0xFF); /* Espacio invertido */
        str++;
    }
    
    i2c_stop();
}
#endif /* SSD1306_USE_TEXT_INV */

#endif /* SSD1306_USE_TEXT */

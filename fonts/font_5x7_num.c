/**
 * font_5x7_num.c - Fuente 5x7 solo números y símbolos básicos
 * 
 * Caracteres: 0-9, :, -, ., espacio = 14 chars × 5 bytes = 70 bytes
 * Ideal para displays numéricos (relojes, contadores)
 */

#include "ssd1306_font.h"

#if SSD1306_USE_FONT_5X7_NUM && !SSD1306_USE_FONT_5X7_FULL && !SSD1306_USE_FONT_5X7_UPPER

/* Fuente 5x7 reducida - solo números y símbolos */
static const uint8_t font5x7_num[] = {
    /* Espacio (32) */
    0x00, 0x00, 0x00, 0x00, 0x00,
    /* - (45) */
    0x08, 0x08, 0x08, 0x08, 0x08,
    /* . (46) */
    0x00, 0x60, 0x60, 0x00, 0x00,
    /* 0-9 (48-57) */
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
    /* : (58) */
    0x00, 0x36, 0x36, 0x00, 0x00,
};

/* Mapeo de índices */
static uint8_t get_num_index(char c) {
    if (c == ' ') return 0;
    if (c == '-') return 1;
    if (c == '.') return 2;
    if (c >= '0' && c <= '9') return 3 + (c - '0');
    if (c == ':') return 13;
    return 0; /* Espacio por defecto */
}

/* Obtener puntero a glyph */
const uint8_t* font_get_glyph(char c) {
    return &font5x7_num[get_num_index(c) * 5];
}

/* Ancho de fuente */
uint8_t font_get_width(void) {
    return 5;
}

#endif /* SSD1306_USE_FONT_5X7_NUM */

/**
 * ssd1306_bigfont.c - Fuente grande estilo pixel art (OPTIMIZADO)
 * 
 * Sistema de píxeles grandes (bloques de 3×3 píxeles)
 * Cada caracter es una matriz de 5×5 "píxeles grandes"
 * Tamaño final: 15×15 píxeles por caracter
 * 
 * Optimizaciones:
 * - Tabla precalculada para divisiones (evita / y % costosos en 6502)
 * - Máscaras precalculadas (evita shifts variables)
 * - Funciones unificadas
 */

#include "ssd1306_bigfont.h"
#include "../core/ssd1306_core.h"
#include "../../i2c/i2c.h"

#if SSD1306_USE_BIGNUM

/* ============================================
 * TABLAS DE OPTIMIZACIÓN
 * ============================================ */

/* Tabla para división por 10: div10[n] = n/10 para n=0..99 */
static const uint8_t div10[100] = {
    0,0,0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,3,3,
    4,4,4,4,4,4,4,4,4,4, 5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6, 7,7,7,7,7,7,7,7,7,7,
    8,8,8,8,8,8,8,8,8,8, 9,9,9,9,9,9,9,9,9,9
};

/* Tabla para módulo 10: mod10[n] = n%10 para n=0..99 */
static const uint8_t mod10[100] = {
    0,1,2,3,4,5,6,7,8,9, 0,1,2,3,4,5,6,7,8,9,
    0,1,2,3,4,5,6,7,8,9, 0,1,2,3,4,5,6,7,8,9,
    0,1,2,3,4,5,6,7,8,9, 0,1,2,3,4,5,6,7,8,9,
    0,1,2,3,4,5,6,7,8,9, 0,1,2,3,4,5,6,7,8,9,
    0,1,2,3,4,5,6,7,8,9, 0,1,2,3,4,5,6,7,8,9
};

/* Máscaras de bit precalculadas (evita 0x01 << n) */
static const uint8_t bit_mask[5] = {0x01, 0x02, 0x04, 0x08, 0x10};

/* ============================================
 * DEFINICIÓN DE CARACTERES 5×5
 * ============================================ */

/* Dígitos 0-9 (5 bytes por carácter = columnas) */
static const uint8_t bigfont_digits[][5] = {
    /* 0 */  {0x0E, 0x11, 0x11, 0x11, 0x0E},
    /* 1 */  {0x00, 0x12, 0x1F, 0x10, 0x00},
    /* 2 */  {0x12, 0x19, 0x15, 0x15, 0x12},
    /* 3 */  {0x0A, 0x11, 0x15, 0x15, 0x0A},
    /* 4 */  {0x07, 0x04, 0x04, 0x1F, 0x04},
    /* 5 */  {0x17, 0x15, 0x15, 0x15, 0x09},
    /* 6 */  {0x0E, 0x15, 0x15, 0x15, 0x08},
    /* 7 */  {0x01, 0x19, 0x05, 0x03, 0x01},
    /* 8 */  {0x0A, 0x15, 0x15, 0x15, 0x0A},
    /* 9 */  {0x02, 0x15, 0x15, 0x15, 0x0E},
};

/* Símbolos : - . espacio */
static const uint8_t bigfont_symbols[][5] = {
    /* : */  {0x00, 0x00, 0x0A, 0x00, 0x00},  /* puntos en filas 1 y 3 */
    /* - */  {0x00, 0x04, 0x04, 0x04, 0x00},  /* línea en fila 2 */
    /* . */  {0x00, 0x00, 0x10, 0x00, 0x00},  /* punto en fila 4 (abajo) */
    /*   */  {0x00, 0x00, 0x00, 0x00, 0x00},
};

#if SSD1306_BIGNUM_LETTERS
/* Letras A-Z (5 bytes por carácter = columnas)
 * Bit 0 = fila 0 (arriba), Bit 4 = fila 4 (abajo)
 */
static const uint8_t bigfont_letters[][5] = {
    /* A */  {0x1E, 0x05, 0x05, 0x05, 0x1E},
    /* B */  {0x1F, 0x15, 0x15, 0x15, 0x0A},
    /* C */  {0x0E, 0x11, 0x11, 0x11, 0x0A},
    /* D */  {0x1F, 0x11, 0x11, 0x11, 0x0E},
    /* E */  {0x1F, 0x15, 0x15, 0x15, 0x11},
    /* F */  {0x1F, 0x05, 0x05, 0x05, 0x01},
    /* G */  {0x0E, 0x11, 0x15, 0x15, 0x0C},
    /* H */  {0x1F, 0x04, 0x04, 0x04, 0x1F},
    /* I */  {0x00, 0x11, 0x1F, 0x11, 0x00},
    /* J */  {0x08, 0x10, 0x10, 0x10, 0x0F},
    /* K */  {0x1F, 0x04, 0x0A, 0x11, 0x00},
    /* L */  {0x1F, 0x10, 0x10, 0x10, 0x10},
    /* M */  {0x1F, 0x02, 0x04, 0x02, 0x1F},
    /* N */  {0x1F, 0x02, 0x04, 0x08, 0x1F},
    /* O */  {0x0E, 0x11, 0x11, 0x11, 0x0E},
    /* P */  {0x1F, 0x05, 0x05, 0x05, 0x02},
    /* Q */  {0x0E, 0x11, 0x19, 0x11, 0x1E},
    /* R */  {0x1F, 0x05, 0x05, 0x0D, 0x12},
    /* S */  {0x12, 0x15, 0x15, 0x15, 0x09},
    /* T */  {0x01, 0x01, 0x1F, 0x01, 0x01},
    /* U */  {0x0F, 0x10, 0x10, 0x10, 0x0F},
    /* V */  {0x03, 0x0C, 0x10, 0x0C, 0x03},
    /* W */  {0x0F, 0x10, 0x0C, 0x10, 0x0F},
    /* X */  {0x11, 0x0A, 0x04, 0x0A, 0x11},
    /* Y */  {0x01, 0x02, 0x1C, 0x02, 0x01},
    /* Z */  {0x19, 0x15, 0x15, 0x15, 0x13},
};
#endif /* SSD1306_BIGNUM_LETTERS */

/* ============================================
 * FUNCIONES INTERNAS
 * ============================================ */

/* Obtener patrón de un caracter (retorna puntero a 5 bytes) */
static const uint8_t* get_char_pattern(char c) {
    if (c >= '0' && c <= '9') {
        return bigfont_digits[c - '0'];
    }
    if (c == ':') return bigfont_symbols[0];
    if (c == '-') return bigfont_symbols[1];
    if (c == '.') return bigfont_symbols[2];
    if (c == ' ') return bigfont_symbols[3];
#if SSD1306_BIGNUM_LETTERS
    if (c >= 'A' && c <= 'Z') {
        return bigfont_letters[c - 'A'];
    }
    if (c >= 'a' && c <= 'z') {
        return bigfont_letters[c - 'a']; /* Minúsculas → mayúsculas */
    }
#endif
    return bigfont_symbols[3]; /* Espacio por defecto */
}

/* 
 * Patrones de bloque para cada fila (3 píxeles de alto)
 * Cada fila de bloques contribuye bits a página 0 y/o página 1
 * 
 * Fila 0: píxeles 0-2  → página 0: bits 0,1,2 = 0x07
 * Fila 1: píxeles 3-5  → página 0: bits 3,4,5 = 0x38
 * Fila 2: píxeles 6-8  → página 0: bits 6,7 = 0xC0, página 1: bit 0 = 0x01
 * Fila 3: píxeles 9-11 → página 1: bits 1,2,3 = 0x0E
 * Fila 4: píxeles 12-14→ página 1: bits 4,5,6 = 0x70
 */

/* Patrón para página 0 por cada fila de bloques */
static const uint8_t row_pattern_p0[5] = {0x07, 0x38, 0xC0, 0x00, 0x00};
/* Patrón para página 1 por cada fila de bloques */
static const uint8_t row_pattern_p1[5] = {0x00, 0x00, 0x01, 0x0E, 0x70};

/* Dibujar un carácter grande (OPTIMIZADO: usa máscaras precalculadas) */
void ssd1306_bigchar(uint8_t x, uint8_t page, char c) {
    const uint8_t *pattern;
    uint8_t data_col;      /* Columna de datos (0-4) */
    uint8_t px_col;        /* Columna de píxeles dentro del bloque (0-2) */
    uint8_t data_row;      /* Fila de datos (0-4) */
    uint8_t col_bits;      /* Bits activos de la columna */
    uint8_t byte_p0, byte_p1;
    
    pattern = get_char_pattern(c);
    
    /* Dibujar página superior */
    ssd1306_set_pos(x, page);
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    
    for (data_col = 0; data_col < 5; data_col++) {
        col_bits = pattern[data_col];
        
        /* Calcular byte para página 0 usando máscaras precalculadas */
        byte_p0 = 0;
        for (data_row = 0; data_row < 5; data_row++) {
            if (col_bits & bit_mask[data_row]) {
                byte_p0 |= row_pattern_p0[data_row];
            }
        }
        
        /* Escribir 3 veces (3 píxeles de ancho por bloque) */
        for (px_col = 0; px_col < 3; px_col++) {
            i2c_write_byte(byte_p0);
        }
    }
    i2c_stop();
    
    /* Dibujar página inferior */
    ssd1306_set_pos(x, page + 1);
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    
    for (data_col = 0; data_col < 5; data_col++) {
        col_bits = pattern[data_col];
        
        /* Calcular byte para página 1 usando máscaras precalculadas */
        byte_p1 = 0;
        for (data_row = 0; data_row < 5; data_row++) {
            if (col_bits & bit_mask[data_row]) {
                byte_p1 |= row_pattern_p1[data_row];
            }
        }
        
        /* Escribir 3 veces */
        for (px_col = 0; px_col < 3; px_col++) {
            i2c_write_byte(byte_p1);
        }
    }
    i2c_stop();
}

/* Limpiar área de un caracter grande */
void ssd1306_bigchar_clear(uint8_t x, uint8_t page) {
    uint8_t i;
    
    /* Limpiar página superior */
    ssd1306_set_pos(x, page);
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    for (i = 0; i < 15; i++) {
        i2c_write_byte(0x00);
    }
    i2c_stop();
    
    /* Limpiar página inferior */
    ssd1306_set_pos(x, page + 1);
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    for (i = 0; i < 15; i++) {
        i2c_write_byte(0x00);
    }
    i2c_stop();
}

/* ============================================
 * FUNCIONES PÚBLICAS - TEXTO
 * ============================================ */

/* Calcular ancho de texto en píxeles */
uint8_t ssd1306_bigtext_width(const char *str) {
    uint8_t len = 0;
    while (*str++) len++;
    if (len == 0) return 0;
    return (len * BIGFONT_CHAR_W) + ((len - 1) * BIGFONT_SPACING);
}

/* Dibujar texto grande en posición específica */
void ssd1306_bigtext(uint8_t x, uint8_t page, const char *str) {
    uint8_t pos = x;
    
    while (*str && pos < 128) {
        ssd1306_bigchar(pos, page, *str);
        pos += BIGFONT_CHAR_W + BIGFONT_SPACING;
        str++;
    }
}

/* Dibujar texto grande centrado verticalmente (1 línea) */
void ssd1306_bigtext_centered(const char *str) {
    /* Centrado vertical: página 1 (y = 8, margen de 8px arriba) */
    ssd1306_bigtext(0, BIGFONT_CENTER, str);
}

/* Dibujar texto centrado horizontal y verticalmente */
void ssd1306_bigtext_center(const char *str) {
    uint8_t width = ssd1306_bigtext_width(str);
    uint8_t x = (128 - width) / 2;
    
    ssd1306_bigtext(x, BIGFONT_CENTER, str);
}

/* ============================================
 * FUNCIONES PÚBLICAS - NÚMEROS
 * ============================================ */

/* Dibujar un dígito (0-9) */
void ssd1306_bignum_digit(uint8_t x, uint8_t page, uint8_t digit) {
    if (digit > 9) digit = 0;
    ssd1306_bigchar(x, page, '0' + digit);
}

/* Dibujar dos puntos */
void ssd1306_bignum_colon(uint8_t x, uint8_t page) {
    ssd1306_bigchar(x, page, ':');
}

/* Dibujar número de 2 dígitos (usa tablas precalculadas) */
void ssd1306_bignum_2digit(uint8_t x, uint8_t page, uint8_t num) {
    if (num > 99) num = 99;
    ssd1306_bigchar(x, page, '0' + div10[num]);
    ssd1306_bigchar(x + BIGFONT_CHAR_W + BIGFONT_SPACING, page, '0' + mod10[num]);
}

/* Dibujar hora HH:MM en posición específica (usa tablas precalculadas) */
void ssd1306_bignum_time(uint8_t x, uint8_t page, uint8_t hours, uint8_t minutes) {
    uint8_t pos = x;
    
    /* HH */
    ssd1306_bigchar(pos, page, '0' + div10[hours]);
    pos += BIGFONT_CHAR_W + BIGFONT_SPACING;
    ssd1306_bigchar(pos, page, '0' + mod10[hours]);
    pos += BIGFONT_CHAR_W + BIGFONT_SPACING;
    
    /* : */
    ssd1306_bigchar(pos, page, ':');
    pos += BIGFONT_CHAR_W + BIGFONT_SPACING;
    
    /* MM */
    ssd1306_bigchar(pos, page, '0' + div10[minutes]);
    pos += BIGFONT_CHAR_W + BIGFONT_SPACING;
    ssd1306_bigchar(pos, page, '0' + mod10[minutes]);
}

/* Dibujar hora HH:MM centrada (constantes precalculadas) */
void ssd1306_bignum_time_centered(uint8_t hours, uint8_t minutes) {
    /* HH:MM = 5 caracteres, ancho = 5*15 + 4*1 = 79px */
    /* x = (128 - 79) / 2 = 24 (constante) */
    ssd1306_bignum_time(24, BIGFONT_CENTER, hours, minutes);
}

/* Dibujar número grande (0-9999) usando tablas */
void ssd1306_bignum_number(uint8_t x, uint8_t page, uint16_t num, uint8_t digits) {
    uint8_t d[4];
    uint8_t i;
    uint8_t pos = x;
    uint8_t h, l;
    
    /* Dividir en high (num/100) y low (num%100) */
    h = num / 100;
    l = num - (h * 100);  /* Más eficiente que % */
    
    d[0] = div10[h];      /* miles */
    d[1] = mod10[h];      /* centenas */
    d[2] = div10[l];      /* decenas */
    d[3] = mod10[l];      /* unidades */
    
    for (i = 4 - digits; i < 4; i++) {
        ssd1306_bigchar(pos, page, '0' + d[i]);
        pos += BIGFONT_CHAR_W + BIGFONT_SPACING;
    }
}

#endif /* SSD1306_USE_BIGNUM */

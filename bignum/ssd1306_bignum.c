/**
 * ssd1306_bignum.c - Números grandes por bloques
 * 
 * Sistema de bloques 8x8 que se combinan para formar dígitos grandes.
 * Mucho más eficiente que almacenar bitmaps completos.
 * 
 * Bloques: 8 bloques × 8 bytes = 64 bytes
 * Mapas: 10 dígitos × 12 bytes = 120 bytes
 * Total: ~184 bytes vs ~640 bytes de bitmap completo
 */

#include "ssd1306_bignum.h"
#include "../core/ssd1306_core.h"
#include "../../i2c/i2c.h"

#if SSD1306_USE_BIGNUM

/* ============================================
 * BLOQUES BASE 8x8
 * ============================================
 * 
 * Cada bloque es una pieza del rompecabezas:
 * 
 *  [0] FULL   [1] EMPTY  [2] LEFT   [3] RIGHT
 *   ████       ....       █...       ...█
 *   ████       ....       █...       ...█
 *   ████       ....       █...       ...█
 *   ████       ....       █...       ...█
 * 
 *  [4] TOP    [5] BOT    [6] HTOP   [7] HBOT
 *   ████       ....       ████       ....
 *   ████       ....       ....       ....
 *   ....       ████       ....       ....
 *   ....       ████       ....       ████
 * 
 *  [8] TL     [9] TR     [10] BL    [11] BR
 *   ███.       .███       ....       ....
 *   █...       ...█       █...       ...█
 *   █...       ...█       ███.       .███
 *   ....       ....       ████       ████
 */

/* Bloques 8x8 para construir dígitos
 * 
 * SSD1306 usa columnas verticales: cada byte = 8 píxeles verticales
 * bit 0 = arriba, bit 7 = abajo
 * 
 * Cada bloque: 8 columnas × 8 filas (1 página de alto)
 */
static const uint8_t blocks[][8] = {
    /* 0: FULL - bloque sólido completo */
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    
    /* 1: EMPTY - vacío */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    
    /* 2: LEFT - barra vertical izquierda (2 cols) */
    {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    
    /* 3: RIGHT - barra vertical derecha (2 cols) */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF},
    
    /* 4: TOP - barra horizontal arriba (2 filas) */
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03},
    
    /* 5: BOTTOM - barra horizontal abajo (2 filas) */
    {0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0},
    
    /* 6: MIDDLE - barra horizontal centro */
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    
    /* 7: TOP-LEFT - esquina superior izquierda */
    {0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03},
    
    /* 8: TOP-RIGHT - esquina superior derecha */
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF},
    
    /* 9: BOT-LEFT - esquina inferior izquierda */
    {0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0},
    
    /* 10: BOT-RIGHT - esquina inferior derecha */
    {0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF},
    
    /* 11: SIDES - barras izq y der */
    {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF},
    
    /* 12: DOT - punto para colon */
    {0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00},
};

/* ============================================
 * MAPAS DE DÍGITOS
 * ============================================
 * 
 * Cada dígito usa una grilla de 3×4 bloques.
 * 12 índices por dígito.
 * 
 * Ejemplo "0":
 *   ┌─────────┐
 *   │ TL  T  TR │  [7][4][8]
 *   │ L   .  R  │  [2][1][3]
 *   │ L   .  R  │  [2][1][3]
 *   │ BL  B  BR │  [9][5][10]
 *   └─────────┘
 */

/* Mapas: cada dígito = 12 índices de bloques (3 columnas × 4 filas)
 * Formato: fila0(3), fila1(3), fila2(3), fila3(3)
 */
static const uint8_t digit_map[12][12] = {
    /* 0:  ███   */
    /*    █   █  */
    /*    █   █  */
    /*     ███   */
    { 7, 4, 8,  11, 1,11,  11, 1,11,   9, 5,10},
    /* 1:    █   */
    /*       █   */
    /*       █   */
    /*       █   */
    { 1, 4, 1,   1, 3, 1,   1, 3, 1,   1, 5, 1},
    /* 2:  ███   */
    /*       █   */
    /*    ███    */
    /*    ███    */
    { 7, 4, 8,   1, 6, 3,   7, 6, 8,   9, 5, 1},
    /* 3:  ███   */
    /*       █   */
    /*     ██    */
    /*    ███    */
    { 7, 4, 8,   1, 6, 3,   1, 6, 3,   9, 5,10},
    /* 4:  █   █ */
    /*    █   █  */
    /*     ███   */
    /*       █   */
    {11, 1,11,  11, 6,11,   1, 5, 3,   1, 1, 3},
    /* 5:  ███   */
    /*    █      */
    /*     ███   */
    /*       ██  */
    { 7, 4, 8,   2, 6, 1,   1, 6, 8,   9, 5,10},
    /* 6:  ███   */
    /*    █      */
    /*    ███    */
    /*    ███    */
    { 7, 4, 8,   2, 6, 1,  11, 6, 8,   9, 5,10},
    /* 7:  ███   */
    /*       █   */
    /*       █   */
    /*       █   */
    { 7, 4, 8,   1, 1, 3,   1, 1, 3,   1, 1, 3},
    /* 8:  ███   */
    /*    █   █  */
    /*    ███    */
    /*    ███    */
    { 7, 4, 8,  11, 6,11,  11, 6,11,   9, 5,10},
    /* 9:  ███   */
    /*    █   █  */
    /*     ███   */
    /*       █   */
    { 7, 4, 8,  11, 6,11,   1, 5, 3,   1, 5, 3},
    /* : (colon) - índice 10 */
    { 1, 1, 1,   1,12, 1,   1, 1, 1,   1,12, 1},
    /* - (minus) - índice 11 */
    { 1, 1, 1,   1, 6, 1,   1, 6, 1,   1, 1, 1},
};

/* Dibujar un bloque 8x8 */
static void draw_block(uint8_t x, uint8_t page, uint8_t block_id) {
    uint8_t i;
    
    ssd1306_set_pos(x, page);
    
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_DATA);
    for (i = 0; i < 8; i++) {
        i2c_write_byte(blocks[block_id][i]);
    }
    i2c_stop();
}

/* ============================================
 * FUNCIONES PÚBLICAS
 * ============================================ */

/* Dibujar un dígito grande (0-9) */
void ssd1306_bignum_digit(uint8_t x, uint8_t digit) {
    uint8_t col, row;
    uint8_t block_idx;
    
    if (digit > 9) digit = 0;
    
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 3; col++) {
            block_idx = digit_map[digit][row * 3 + col];
            draw_block(x + col * 8, row, block_idx);
        }
    }
}

/* Dibujar dos puntos ":" para reloj */
void ssd1306_bignum_colon(uint8_t x) {
    uint8_t row;
    
    for (row = 0; row < 4; row++) {
        draw_block(x, row, digit_map[10][row * 3 + 1]);
    }
}

/* Dibujar número de 2 dígitos (00-99) */
void ssd1306_bignum_2digit(uint8_t x, uint8_t num) {
    if (num > 99) num = 99;
    
    ssd1306_bignum_digit(x, num / 10);
    ssd1306_bignum_digit(x + BIGNUM_WIDTH + BIGNUM_SPACING, num % 10);
}

/* Dibujar hora HH:MM */
void ssd1306_bignum_time(uint8_t x, uint8_t hours, uint8_t minutes) {
    uint8_t pos = x;
    
    /* Horas */
    ssd1306_bignum_digit(pos, hours / 10);
    pos += BIGNUM_WIDTH + BIGNUM_SPACING;
    ssd1306_bignum_digit(pos, hours % 10);
    pos += BIGNUM_WIDTH + BIGNUM_SPACING;
    
    /* Colon */
    ssd1306_bignum_colon(pos);
    pos += 8 + BIGNUM_SPACING;
    
    /* Minutos */
    ssd1306_bignum_digit(pos, minutes / 10);
    pos += BIGNUM_WIDTH + BIGNUM_SPACING;
    ssd1306_bignum_digit(pos, minutes % 10);
}

/* Dibujar número grande (0-9999) */
void ssd1306_bignum_number(uint8_t x, uint16_t num, uint8_t digits) {
    uint8_t i;
    uint8_t d[4];
    uint8_t pos = x;
    
    /* Extraer dígitos */
    d[0] = (num / 1000) % 10;
    d[1] = (num / 100) % 10;
    d[2] = (num / 10) % 10;
    d[3] = num % 10;
    
    /* Dibujar según cantidad de dígitos solicitados */
    for (i = 4 - digits; i < 4; i++) {
        ssd1306_bignum_digit(pos, d[i]);
        pos += BIGNUM_WIDTH + BIGNUM_SPACING;
    }
}

#endif /* SSD1306_USE_BIGNUM */

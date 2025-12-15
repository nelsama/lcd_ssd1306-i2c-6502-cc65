/**
 * ssd1306_bigfont.h - Fuente grande pixel art (5x5 bloques, 3x3 píxeles)
 * 
 * Cada caracter: 5 columnas × 5 filas de bloques
 * Cada bloque: 3×3 píxeles
 * Tamaño final: 15×15 píxeles por caracter
 * 
 * Caben 8 caracteres por línea, 2 líneas en pantalla
 * Función de centrado vertical para línea única
 */

#ifndef SSD1306_BIGFONT_H
#define SSD1306_BIGFONT_H

#include <stdint.h>
#include "../ssd1306_config.h"

#if SSD1306_USE_BIGNUM

/* ============================================
 * DIMENSIONES
 * ============================================ */
#define BIGFONT_COLS      5       /* Columnas de bloques por caracter */
#define BIGFONT_ROWS      5       /* Filas de bloques por caracter */
#define BIGFONT_BLOCK_W   3       /* Ancho de cada bloque en píxeles */
#define BIGFONT_BLOCK_H   3       /* Alto de cada bloque en píxeles */
#define BIGFONT_CHAR_W    (BIGFONT_COLS * BIGFONT_BLOCK_W)  /* 15 píxeles */
#define BIGFONT_CHAR_H    (BIGFONT_ROWS * BIGFONT_BLOCK_H)  /* 15 píxeles */
#define BIGFONT_SPACING   1       /* Espacio entre caracteres */

/* Posiciones de fila (páginas SSD1306) */
#define BIGFONT_ROW1      0       /* Fila superior: página 0 */
#define BIGFONT_ROW2      2       /* Fila inferior: página 2 */
#define BIGFONT_CENTER    1       /* Fila centrada: página 1 */

/* ============================================
 * FUNCIONES DE TEXTO
 * ============================================ */

/* Dibujar un caracter grande en posición específica */
void ssd1306_bigchar(uint8_t x, uint8_t page, char c);

/* Dibujar texto grande en posición específica */
void ssd1306_bigtext(uint8_t x, uint8_t page, const char *str);

/* Dibujar texto grande centrado verticalmente (1 línea) */
void ssd1306_bigtext_centered(const char *str);

/* Dibujar texto centrado horizontal y verticalmente */
void ssd1306_bigtext_center(const char *str);

/* ============================================
 * FUNCIONES DE NÚMEROS (compatibilidad)
 * ============================================ */

/* Dibujar un dígito grande (0-9) */
void ssd1306_bignum_digit(uint8_t x, uint8_t page, uint8_t digit);

/* Dibujar dos puntos ":" */
void ssd1306_bignum_colon(uint8_t x, uint8_t page);

/* Dibujar hora HH:MM en posición específica */
void ssd1306_bignum_time(uint8_t x, uint8_t page, uint8_t hours, uint8_t minutes);

/* Dibujar hora HH:MM centrada */
void ssd1306_bignum_time_centered(uint8_t hours, uint8_t minutes);

/* Dibujar hora HH:MM:SS centrada */
void ssd1306_bignum_time_full(uint8_t hours, uint8_t minutes, uint8_t seconds);

/* Dibujar número de 2 dígitos (00-99) */
void ssd1306_bignum_2digit(uint8_t x, uint8_t page, uint8_t num);

/* Dibujar número grande (0-9999) */
void ssd1306_bignum_number(uint8_t x, uint8_t page, uint16_t num, uint8_t digits);

/* ============================================
 * UTILIDADES
 * ============================================ */

/* Calcular ancho de texto en píxeles */
uint8_t ssd1306_bigtext_width(const char *str);

/* Limpiar área de un caracter grande */
void ssd1306_bigchar_clear(uint8_t x, uint8_t page);

#endif /* SSD1306_USE_BIGNUM */

#endif /* SSD1306_BIGFONT_H */

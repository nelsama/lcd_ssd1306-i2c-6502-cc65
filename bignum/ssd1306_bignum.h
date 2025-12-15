/**
 * ssd1306_bignum.h - Números grandes por bloques
 * 
 * Dígitos de 24x32 píxeles (3 cols × 4 páginas)
 * Ideal para relojes y contadores
 */

#ifndef SSD1306_BIGNUM_H
#define SSD1306_BIGNUM_H

#include <stdint.h>
#include <ssd1306/ssd1306_config.h>

#if SSD1306_USE_BIGNUM

/* Dimensiones de dígitos grandes */
#define BIGNUM_WIDTH    24      /* 3 bloques de 8 píxeles */
#define BIGNUM_HEIGHT   4       /* 4 páginas = 32 píxeles */
#define BIGNUM_SPACING  2       /* Espacio entre dígitos */

/* Dibujar un dígito grande (0-9) */
void ssd1306_bignum_digit(uint8_t x, uint8_t digit);

/* Dibujar dos puntos ":" para reloj */
void ssd1306_bignum_colon(uint8_t x);

/* Dibujar número de 2 dígitos (00-99) */
void ssd1306_bignum_2digit(uint8_t x, uint8_t num);

/* Dibujar hora HH:MM */
void ssd1306_bignum_time(uint8_t x, uint8_t hours, uint8_t minutes);

/* Dibujar número grande (0-9999) */
void ssd1306_bignum_number(uint8_t x, uint16_t num, uint8_t digits);

#endif /* SSD1306_USE_BIGNUM */

#endif /* SSD1306_BIGNUM_H */

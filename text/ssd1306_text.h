/**
 * ssd1306_text.h - Funciones de texto
 */

#ifndef SSD1306_TEXT_H
#define SSD1306_TEXT_H

#include <stdint.h>
#include <ssd1306/ssd1306_config.h>

#if SSD1306_USE_TEXT

/* Escribir un caracter en posición */
void ssd1306_char(uint8_t col, uint8_t page, char c);

/* Escribir texto en posición */
void ssd1306_text(uint8_t col, uint8_t page, const char *str);

#if SSD1306_USE_TEXT_INV
/* Escribir texto invertido (blanco sobre negro) */
void ssd1306_text_inv(uint8_t col, uint8_t page, const char *str);
#endif

#endif /* SSD1306_USE_TEXT */

#endif /* SSD1306_TEXT_H */

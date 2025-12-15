/**
 * ssd1306_numbers.h - Funciones de números
 */

#ifndef SSD1306_NUMBERS_H
#define SSD1306_NUMBERS_H

#include <stdint.h>
#include "../ssd1306_config.h"

#if SSD1306_USE_NUMBERS

/* Mostrar número decimal sin signo (0-65535) */
void ssd1306_number(uint8_t col, uint8_t page, uint16_t num);

/* Mostrar número con signo (-32768 a 32767) */
void ssd1306_number_signed(uint8_t col, uint8_t page, int16_t num);

#endif /* SSD1306_USE_NUMBERS */

#if SSD1306_USE_NUMBERS_HEX

/* Mostrar número hexadecimal (8 bits) */
void ssd1306_hex8(uint8_t col, uint8_t page, uint8_t num);

/* Mostrar número hexadecimal (16 bits) */
void ssd1306_hex16(uint8_t col, uint8_t page, uint16_t num);

#endif /* SSD1306_USE_NUMBERS_HEX */

#endif /* SSD1306_NUMBERS_H */

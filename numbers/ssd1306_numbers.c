/**
 * ssd1306_numbers.c - Funciones de números
 */

#include "ssd1306_numbers.h"
#include "../text/ssd1306_text.h"

#if SSD1306_USE_NUMBERS

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

#endif /* SSD1306_USE_NUMBERS */

#if SSD1306_USE_NUMBERS_HEX

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

#endif /* SSD1306_USE_NUMBERS_HEX */

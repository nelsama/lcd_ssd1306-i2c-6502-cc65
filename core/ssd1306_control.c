/**
 * ssd1306_control.c - Control de display
 */

#include "ssd1306_control.h"
#include "ssd1306_core.h"

#if SSD1306_USE_CONTROL

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

#endif /* SSD1306_USE_CONTROL */

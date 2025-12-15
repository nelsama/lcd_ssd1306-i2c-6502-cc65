/**
 * ssd1306_control.h - Control de display
 */

#ifndef SSD1306_CONTROL_H
#define SSD1306_CONTROL_H

#include <stdint.h>
#include "../ssd1306_config.h"

#if SSD1306_USE_CONTROL

/* Ajustar contraste (0-255) */
void ssd1306_set_contrast(uint8_t contrast);

/* Invertir colores (0=normal, 1=invertido) */
void ssd1306_invert(uint8_t invert);

/* Flip horizontal */
void ssd1306_flip_h(uint8_t flip);

/* Flip vertical */
void ssd1306_flip_v(uint8_t flip);

#endif /* SSD1306_USE_CONTROL */

#endif /* SSD1306_CONTROL_H */

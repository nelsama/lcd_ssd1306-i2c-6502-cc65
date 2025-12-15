/**
 * ssd1306_font.h - Header común para fuentes
 */

#ifndef SSD1306_FONT_H
#define SSD1306_FONT_H

#include <stdint.h>
#include "../ssd1306_config.h"

/* Obtener puntero a glyph de un caracter */
const uint8_t* font_get_glyph(char c);

/* Obtener ancho de la fuente activa */
uint8_t font_get_width(void);

#endif /* SSD1306_FONT_H */

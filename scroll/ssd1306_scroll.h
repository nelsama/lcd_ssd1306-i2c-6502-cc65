/**
 * ssd1306_scroll.h - Funciones de scroll
 */

#ifndef SSD1306_SCROLL_H
#define SSD1306_SCROLL_H

#include <stdint.h>
#include <ssd1306/ssd1306_config.h>

#if SSD1306_USE_SCROLL

/* Scroll derecha */
void ssd1306_scroll_right(uint8_t start_page, uint8_t end_page, uint8_t speed);

/* Scroll izquierda */
void ssd1306_scroll_left(uint8_t start_page, uint8_t end_page, uint8_t speed);

/* Detener scroll */
void ssd1306_scroll_stop(void);

#endif /* SSD1306_USE_SCROLL */

#endif /* SSD1306_SCROLL_H */

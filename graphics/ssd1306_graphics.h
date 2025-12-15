/**
 * ssd1306_graphics.h - Funciones gráficas
 */

#ifndef SSD1306_GRAPHICS_H
#define SSD1306_GRAPHICS_H

#include <stdint.h>
#include <ssd1306/ssd1306_config.h>

#if SSD1306_USE_GRAPHICS

/* Dibujar línea horizontal */
void ssd1306_hline(uint8_t x, uint8_t page, uint8_t width, uint8_t pattern);

/* Llenar una página completa con patrón */
void ssd1306_fill_page(uint8_t page, uint8_t pattern);

#endif /* SSD1306_USE_GRAPHICS */

#if SSD1306_USE_RECT

/* Dibujar rectángulo */
void ssd1306_rect(uint8_t x, uint8_t page, uint8_t width, uint8_t height_pages);

#endif /* SSD1306_USE_RECT */

#if SSD1306_USE_PROGRESS

/* Dibujar barra de progreso (0-100%) */
void ssd1306_progress_bar(uint8_t x, uint8_t page, uint8_t width, uint8_t percent);

#endif /* SSD1306_USE_PROGRESS */

#if SSD1306_USE_ICONS

/* Dibujar bitmap 8x8 */
void ssd1306_icon8(uint8_t col, uint8_t page, const uint8_t *icon);

/* Limpiar área rectangular */
void ssd1306_clear_area(uint8_t col, uint8_t page, uint8_t width);

#endif /* SSD1306_USE_ICONS */

#endif /* SSD1306_GRAPHICS_H */

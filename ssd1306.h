/**
 * ssd1306.h - Header maestro modular SSD1306
 * 
 * Incluye automáticamente los módulos activados en ssd1306_config.h
 * 
 * Uso:
 *   1. Edita ssd1306_config.h para activar/desactivar módulos
 *   2. Incluye solo este archivo: #include "ssd1306/ssd1306.h"
 *   3. Actualiza el Makefile para compilar los módulos necesarios
 */

#ifndef SSD1306_H
#define SSD1306_H

/* Configuración */
#include "ssd1306_config.h"

/* Core (siempre necesario) */
#if SSD1306_USE_CORE
#include "core/ssd1306_core.h"
#endif

/* Control de display */
#if SSD1306_USE_CONTROL
#include "core/ssd1306_control.h"
#endif

/* Fuentes */
#if SSD1306_USE_FONT_5X7_FULL || SSD1306_USE_FONT_5X7_UPPER || SSD1306_USE_FONT_5X7_NUM
#include "fonts/ssd1306_font.h"
#endif

/* Texto */
#if SSD1306_USE_TEXT
#include "text/ssd1306_text.h"
#endif

/* Números pequeños */
#if SSD1306_USE_NUMBERS || SSD1306_USE_NUMBERS_HEX
#include "numbers/ssd1306_numbers.h"
#endif

/* Números grandes (fuente pixel art) */
#if SSD1306_USE_BIGNUM
#include "bignum/ssd1306_bigfont.h"
#endif

/* Gráficos */
#if SSD1306_USE_GRAPHICS || SSD1306_USE_RECT || SSD1306_USE_PROGRESS || SSD1306_USE_ICONS
#include "graphics/ssd1306_graphics.h"
#endif

/* Scroll */
#if SSD1306_USE_SCROLL
#include "scroll/ssd1306_scroll.h"
#endif

#endif /* SSD1306_H */

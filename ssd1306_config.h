/**
 * ssd1306_config.h - Configuración del driver SSD1306
 * 
 * Todos los valores pueden ser sobrescritos desde el proyecto
 * definiendo los macros ANTES de incluir este archivo.
 * 
 * Opción 1: En tu código antes de incluir:
 *   #define SSD1306_USE_TEXT 1
 *   #include "ssd1306/ssd1306.h"
 * 
 * Opción 2 (recomendada): Crear ssd1306_config.h en tu proyecto
 *   y compilar con -I<tu_proyecto> ANTES de -I<libs>
 */

#ifndef SSD1306_CONFIG_H
#define SSD1306_CONFIG_H

/* ============================================
 * HARDWARE
 * ============================================ */
#ifndef SSD1306_ADDR
#define SSD1306_ADDR        0x3C    /* Dirección I2C (0x3C o 0x3D) */
#endif

#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH       128
#endif

#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT      32      /* 32 o 64 */
#endif

#ifndef SSD1306_PAGES
#define SSD1306_PAGES       4       /* HEIGHT/8 */
#endif

/* ============================================
 * MÓDULOS - Por defecto todos desactivados
 * ============================================ */

/* CORE: Obligatorio (init, clear, on/off) */
#ifndef SSD1306_USE_CORE
#define SSD1306_USE_CORE            1
#endif

/* TEXTO PEQUEÑO 5x7 */
#ifndef SSD1306_USE_FONT_5X7_FULL
#define SSD1306_USE_FONT_5X7_FULL   0   /* ASCII 32-126 (~475 bytes) */
#endif

#ifndef SSD1306_USE_TEXT
#define SSD1306_USE_TEXT            0   /* ssd1306_text() */
#endif

#ifndef SSD1306_USE_TEXT_INV
#define SSD1306_USE_TEXT_INV        0   /* ssd1306_text_inv() */
#endif

/* NÚMEROS PEQUEÑOS */
#ifndef SSD1306_USE_NUMBERS
#define SSD1306_USE_NUMBERS         0   /* ssd1306_number() */
#endif

#ifndef SSD1306_USE_NUMBERS_HEX
#define SSD1306_USE_NUMBERS_HEX     0   /* ssd1306_hex8/16() */
#endif

/* NÚMEROS GRANDES 15x15px */
#ifndef SSD1306_USE_BIGNUM
#define SSD1306_USE_BIGNUM          0   /* Dígitos 0-9 y símbolos */
#endif

#ifndef SSD1306_BIGNUM_LETTERS
#define SSD1306_BIGNUM_LETTERS      0   /* Letras A-Z (~130 bytes) */
#endif

/* GRÁFICOS DIRECTOS (sin framebuffer) */
#ifndef SSD1306_USE_GRAPHICS
#define SSD1306_USE_GRAPHICS        0   /* ssd1306_hline() */
#endif

#ifndef SSD1306_USE_RECT
#define SSD1306_USE_RECT            0   /* ssd1306_rect() */
#endif

#ifndef SSD1306_USE_PROGRESS
#define SSD1306_USE_PROGRESS        0   /* ssd1306_progress_bar() */
#endif

#ifndef SSD1306_USE_ICONS
#define SSD1306_USE_ICONS           0   /* ssd1306_icon8() */
#endif

/* EXTRAS */
#ifndef SSD1306_USE_SCROLL
#define SSD1306_USE_SCROLL          0   /* Scroll horizontal */
#endif

#ifndef SSD1306_USE_CONTROL
#define SSD1306_USE_CONTROL         0   /* Contraste, inversión */
#endif

/* ============================================
 * FRAMEBUFFER - Manipulación de píxeles
 * Requiere 512 bytes de RAM (+128 si FB_PLOT)
 * ============================================ */
#ifndef SSD1306_USE_FRAMEBUFFER
#define SSD1306_USE_FRAMEBUFFER     0   /* Buffer básico + set/clear pixel */
#endif

#ifndef SSD1306_USE_FB_LINE
#define SSD1306_USE_FB_LINE         0   /* Líneas diagonales (Bresenham) + rect */
#endif

#ifndef SSD1306_USE_FB_CIRCLE
#define SSD1306_USE_FB_CIRCLE       0   /* Círculos (Midpoint) */
#endif

#ifndef SSD1306_USE_FB_FILL
#define SSD1306_USE_FB_FILL         0   /* Rectángulos/círculos rellenos */
#endif

#ifndef SSD1306_USE_FB_PLOT
#define SSD1306_USE_FB_PLOT         0   /* Gráficas de datos (+128 bytes RAM) */
#endif

#ifndef SSD1306_USE_FB_SPRITE
#define SSD1306_USE_FB_SPRITE       0   /* Sprites con OR/XOR */
#endif

/* No usados (para compatibilidad) */
#ifndef SSD1306_USE_FONT_5X7_UPPER
#define SSD1306_USE_FONT_5X7_UPPER  0
#endif

#ifndef SSD1306_USE_FONT_5X7_NUM
#define SSD1306_USE_FONT_5X7_NUM    0
#endif

#ifndef SSD1306_USE_FONT_3X5
#define SSD1306_USE_FONT_3X5        0
#endif

#endif /* SSD1306_CONFIG_H */

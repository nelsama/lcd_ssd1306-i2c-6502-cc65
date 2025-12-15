/**
 * ssd1306_config.h - Configuración del driver SSD1306
 * 
 * Activa (1) o desactiva (0) cada módulo según necesites.
 * Esto reduce el tamaño de la ROM al no compilar código innecesario.
 * 
 * Ejemplos de configuración:
 * 
 *   Solo reloj BigFont (~2KB):
 *     CORE=1, BIGNUM=1, todo lo demás=0
 * 
 *   Texto básico (~1.5KB):
 *     CORE=1, FONT_5X7_FULL=1, TEXT=1, todo lo demás=0
 * 
 *   Completo (~6KB):
 *     Todo en 1
 */

#ifndef SSD1306_CONFIG_H
#define SSD1306_CONFIG_H

/* ============================================
 * HARDWARE
 * ============================================ */
#define SSD1306_ADDR        0x3C    /* Dirección I2C (0x3C o 0x3D) */
#define SSD1306_WIDTH       128
#define SSD1306_HEIGHT      32      /* 32 o 64 */
#define SSD1306_PAGES       4       /* HEIGHT/8 */

/* ============================================
 * MÓDULOS - Cambiar 1/0 según necesites
 * ============================================ */

/* CORE: Obligatorio (init, clear, on/off) */
#define SSD1306_USE_CORE            1

/* TEXTO PEQUEÑO 5x7 */
#define SSD1306_USE_FONT_5X7_FULL   1   /* ASCII 32-126 (~475 bytes) */
#define SSD1306_USE_TEXT            1   /* ssd1306_text() */
#define SSD1306_USE_TEXT_INV        1   /* ssd1306_text_inv() */

/* NÚMEROS PEQUEÑOS */
#define SSD1306_USE_NUMBERS         1   /* ssd1306_number() */
#define SSD1306_USE_NUMBERS_HEX     1   /* ssd1306_hex8/16() */

/* NÚMEROS GRANDES 15x15px */
#define SSD1306_USE_BIGNUM          1   /* Dígitos 0-9 y símbolos */
#define SSD1306_BIGNUM_LETTERS      1   /* Letras A-Z (~130 bytes) */

/* GRÁFICOS */
#define SSD1306_USE_GRAPHICS        1   /* ssd1306_hline() */
#define SSD1306_USE_RECT            1   /* ssd1306_rect() */
#define SSD1306_USE_PROGRESS        1   /* ssd1306_progress_bar() */
#define SSD1306_USE_ICONS           1   /* ssd1306_icon8() */

/* EXTRAS */
#define SSD1306_USE_SCROLL          1   /* Scroll horizontal */
#define SSD1306_USE_CONTROL         1   /* Contraste, inversión */

/* ============================================
 * FRAMEBUFFER - Manipulación de píxeles
 * Requiere 512 bytes de RAM
 * ============================================ */
#define SSD1306_USE_FRAMEBUFFER     1   /* Buffer básico + set/clear pixel */
#define SSD1306_USE_FB_LINE         1   /* Líneas diagonales (Bresenham) */
#define SSD1306_USE_FB_CIRCLE       1   /* Círculos */
#define SSD1306_USE_FB_FILL         1   /* Rectángulos/círculos rellenos */
#define SSD1306_USE_FB_PLOT         1   /* Gráficas de datos (+128 bytes) */
#define SSD1306_USE_FB_SPRITE       0   /* Sprites con XOR/transparencia */

/* No usados (para compatibilidad) */
#define SSD1306_USE_FONT_5X7_UPPER  0
#define SSD1306_USE_FONT_5X7_NUM    0
#define SSD1306_USE_FONT_3X5        0

#endif /* SSD1306_CONFIG_H */

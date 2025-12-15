/**
 * ssd1306_config_fb_demo.h - Configuración para Demo Framebuffer
 * 
 * Configuración MÍNIMA para demostrar el framebuffer:
 *   - Core (obligatorio)
 *   - Framebuffer completo (líneas, círculos, rellenos, sprites, plot)
 *   - Sin texto, sin números, sin BigFont
 * 
 * ROM estimada: ~2.5 KB
 * RAM: 512 bytes (framebuffer) + 128 bytes (plot buffer si activo)
 */

#ifndef SSD1306_CONFIG_H
#define SSD1306_CONFIG_H

/* ============================================
 * HARDWARE
 * ============================================ */
#define SSD1306_ADDR        0x3C
#define SSD1306_WIDTH       128
#define SSD1306_HEIGHT      32
#define SSD1306_PAGES       4

/* ============================================
 * MÓDULOS ACTIVOS
 * ============================================ */

/* CORE: Obligatorio */
#define SSD1306_USE_CORE            1

/* TEXTO: Desactivado para esta demo */
#define SSD1306_USE_FONT_5X7_FULL   0
#define SSD1306_USE_FONT_5X7_UPPER  0
#define SSD1306_USE_FONT_5X7_NUM    0
#define SSD1306_USE_FONT_3X5        0
#define SSD1306_USE_TEXT            0
#define SSD1306_USE_TEXT_INV        0

/* NÚMEROS: Desactivado */
#define SSD1306_USE_NUMBERS         0
#define SSD1306_USE_NUMBERS_HEX     0

/* BIGNUM: Desactivado */
#define SSD1306_USE_BIGNUM          0
#define SSD1306_BIGNUM_LETTERS      0

/* GRÁFICOS DIRECTOS: Desactivado (usamos framebuffer) */
#define SSD1306_USE_GRAPHICS        0
#define SSD1306_USE_RECT            0
#define SSD1306_USE_PROGRESS        0
#define SSD1306_USE_ICONS           0

/* EXTRAS: Desactivado */
#define SSD1306_USE_SCROLL          0
#define SSD1306_USE_CONTROL         0

/* ============================================
 * FRAMEBUFFER - TODO ACTIVO PARA DEMO
 * ============================================ */
#define SSD1306_USE_FRAMEBUFFER     1   /* Buffer 512 bytes */
#define SSD1306_USE_FB_LINE         1   /* Líneas Bresenham + rect */
#define SSD1306_USE_FB_CIRCLE       1   /* Círculos Midpoint */
#define SSD1306_USE_FB_FILL         1   /* Rectángulos/círculos rellenos */
#define SSD1306_USE_FB_PLOT         1   /* Gráficas (+128 bytes RAM) */
#define SSD1306_USE_FB_SPRITE       1   /* Sprites OR/XOR */

#endif /* SSD1306_CONFIG_H */

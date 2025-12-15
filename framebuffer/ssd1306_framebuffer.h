/**
 * ssd1306_framebuffer.h - Frame buffer para SSD1306
 * 
 * Permite manipulación de píxeles individuales.
 * Requiere 512 bytes de RAM para display 128x32.
 * 
 * Módulos opcionales (configurar en ssd1306_config.h):
 *   SSD1306_USE_FRAMEBUFFER  - Buffer básico + set/clear pixel
 *   SSD1306_USE_FB_LINE      - Líneas diagonales (Bresenham)
 *   SSD1306_USE_FB_CIRCLE    - Círculos
 *   SSD1306_USE_FB_FILL      - Rectángulos/círculos rellenos
 *   SSD1306_USE_FB_PLOT      - Gráficas de datos
 */

#ifndef SSD1306_FRAMEBUFFER_H
#define SSD1306_FRAMEBUFFER_H

#include <stdint.h>
#include <ssd1306/ssd1306_config.h>

#if SSD1306_USE_FRAMEBUFFER

/* ============================================
 * BUFFER BÁSICO (siempre incluido con FRAMEBUFFER)
 * ============================================ */

/** Inicializar framebuffer (llenar con 0) */
void fb_init(void);

/** Limpiar framebuffer (todos los píxeles apagados) */
void fb_clear(void);

/** Llenar framebuffer (todos los píxeles encendidos) */
void fb_fill(void);

/** Enviar framebuffer al display */
void fb_flush(void);

/** Setear un píxel (encender) */
void fb_set_pixel(uint8_t x, uint8_t y);

/** Limpiar un píxel (apagar) */
void fb_clear_pixel(uint8_t x, uint8_t y);

/** Toggle un píxel (XOR) */
void fb_toggle_pixel(uint8_t x, uint8_t y);

/** Leer estado de un píxel (1=encendido, 0=apagado) */
uint8_t fb_get_pixel(uint8_t x, uint8_t y);

/** Acceso directo al buffer (para funciones avanzadas) */
uint8_t* fb_get_buffer(void);

/* ============================================
 * LÍNEAS (requiere SSD1306_USE_FB_LINE)
 * ============================================ */
#if SSD1306_USE_FB_LINE

/** Línea entre dos puntos (Bresenham) */
void fb_line(int8_t x0, int8_t y0, int8_t x1, int8_t y1);

/** Línea horizontal (optimizada) */
void fb_hline(uint8_t x, uint8_t y, uint8_t width);

/** Línea vertical (optimizada) */
void fb_vline(uint8_t x, uint8_t y, uint8_t height);

#endif /* SSD1306_USE_FB_LINE */

/* ============================================
 * CÍRCULOS (requiere SSD1306_USE_FB_CIRCLE)
 * ============================================ */
#if SSD1306_USE_FB_CIRCLE

/** Círculo (solo borde) */
void fb_circle(int8_t cx, int8_t cy, uint8_t r);

#if SSD1306_USE_FB_FILL
/** Círculo relleno */
void fb_circle_filled(int8_t cx, int8_t cy, uint8_t r);
#endif

#endif /* SSD1306_USE_FB_CIRCLE */

/* ============================================
 * RECTÁNGULOS (requiere SSD1306_USE_FB_LINE)
 * ============================================ */
#if SSD1306_USE_FB_LINE

/** Rectángulo (solo borde) */
void fb_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

#if SSD1306_USE_FB_FILL
/** Rectángulo relleno */
void fb_rect_filled(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
#endif

#endif /* SSD1306_USE_FB_LINE */

/* ============================================
 * GRÁFICAS (requiere SSD1306_USE_FB_PLOT)
 * ============================================ */
#if SSD1306_USE_FB_PLOT

/** Graficar array de datos (0-31 para altura) */
void fb_plot(const uint8_t *data, uint8_t count, uint8_t x_offset);

/** Graficar con escala automática */
void fb_plot_scaled(const uint8_t *data, uint8_t count, uint8_t min_val, uint8_t max_val);

/** Añadir punto a gráfica con scroll */
void fb_plot_scroll(uint8_t value, uint8_t min_val, uint8_t max_val);

#endif /* SSD1306_USE_FB_PLOT */

/* ============================================
 * SPRITES (requiere SSD1306_USE_FB_SPRITE)
 * ============================================ */
#if SSD1306_USE_FB_SPRITE

/** Dibujar sprite con OR (píxeles se suman) */
void fb_sprite(uint8_t x, uint8_t y, const uint8_t *sprite, uint8_t w, uint8_t h);

/** Dibujar sprite con XOR (para cursor/parpadeo) */
void fb_sprite_xor(uint8_t x, uint8_t y, const uint8_t *sprite, uint8_t w, uint8_t h);

/** Borrar área de sprite */
void fb_sprite_clear(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

#endif /* SSD1306_USE_FB_SPRITE */

#endif /* SSD1306_USE_FRAMEBUFFER */

#endif /* SSD1306_FRAMEBUFFER_H */

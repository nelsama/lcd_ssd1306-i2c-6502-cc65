/**
 * ssd1306.h - Driver completo SSD1306 para 6502
 * Sin framebuffer - escribe directo al display
 * Para displays 128x32
 */

#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

/* Dirección I2C del display */
#define SSD1306_ADDR    0x3C

/* Dimensiones */
#define SSD1306_WIDTH   128
#define SSD1306_HEIGHT  32
#define SSD1306_PAGES   4

/* ============================================
 * FUNCIONES BÁSICAS
 * ============================================ */

/* Inicializar display */
uint8_t ssd1306_init(void);

/* Limpiar pantalla completa */
void ssd1306_clear(void);

/* Encender/apagar display */
void ssd1306_display_on(void);
void ssd1306_display_off(void);

/* ============================================
 * CONTROL DE DISPLAY
 * ============================================ */

/* Ajustar contraste (0-255) */
void ssd1306_set_contrast(uint8_t contrast);

/* Invertir colores (0=normal, 1=invertido) */
void ssd1306_invert(uint8_t invert);

/* Flip horizontal/vertical */
void ssd1306_flip_h(uint8_t flip);
void ssd1306_flip_v(uint8_t flip);

/* Scroll horizontal */
void ssd1306_scroll_right(uint8_t start_page, uint8_t end_page, uint8_t speed);
void ssd1306_scroll_left(uint8_t start_page, uint8_t end_page, uint8_t speed);
void ssd1306_scroll_stop(void);

/* ============================================
 * TEXTO
 * ============================================ */

/* Escribir texto en posición (col 0-127, page 0-3) */
void ssd1306_text(uint8_t col, uint8_t page, const char *str);

/* Escribir texto invertido (blanco sobre negro) */
void ssd1306_text_inv(uint8_t col, uint8_t page, const char *str);

/* Escribir un solo caracter */
void ssd1306_char(uint8_t col, uint8_t page, char c);

/* ============================================
 * NÚMEROS
 * ============================================ */

/* Mostrar número decimal sin signo (0-65535) */
void ssd1306_number(uint8_t col, uint8_t page, uint16_t num);

/* Mostrar número con signo (-32768 a 32767) */
void ssd1306_number_signed(uint8_t col, uint8_t page, int16_t num);

/* Mostrar número hexadecimal (8 bits) */
void ssd1306_hex8(uint8_t col, uint8_t page, uint8_t num);

/* Mostrar número hexadecimal (16 bits) */
void ssd1306_hex16(uint8_t col, uint8_t page, uint16_t num);

/* ============================================
 * GRÁFICOS BÁSICOS
 * ============================================ */

/* Dibujar línea horizontal */
void ssd1306_hline(uint8_t x, uint8_t page, uint8_t width, uint8_t pattern);

/* Llenar una página completa con patrón */
void ssd1306_fill_page(uint8_t page, uint8_t pattern);

/* Dibujar rectángulo (solo borde superior/inferior en páginas) */
void ssd1306_rect(uint8_t x, uint8_t page, uint8_t width, uint8_t height_pages);

/* Dibujar barra de progreso (0-100%) */
void ssd1306_progress_bar(uint8_t x, uint8_t page, uint8_t width, uint8_t percent);

/* ============================================
 * ICONOS Y SÍMBOLOS
 * ============================================ */

/* Dibujar bitmap 8x8 */
void ssd1306_icon8(uint8_t col, uint8_t page, const uint8_t *icon);

/* Limpiar área rectangular */
void ssd1306_clear_area(uint8_t col, uint8_t page, uint8_t width);

/* ============================================
 * COMPATIBILIDAD (alias para versión mini)
 * ============================================ */
#define ssd1306_mini_init()         ssd1306_init()
#define ssd1306_mini_clear()        ssd1306_clear()
#define ssd1306_mini_text(c,p,s)    ssd1306_text(c,p,s)
#define ssd1306_mini_number(c,p,n)  ssd1306_number(c,p,n)

#endif

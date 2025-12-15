/**
 * ssd1306_core.h - Funciones básicas SSD1306
 * 
 * Módulo obligatorio: init, clear, on/off, posicionamiento
 */

#ifndef SSD1306_CORE_H
#define SSD1306_CORE_H

#include <stdint.h>
#include <ssd1306/ssd1306_config.h>

/* Comandos SSD1306 */
#define SSD1306_CMD         0x00
#define SSD1306_DATA        0x40

/* ============================================
 * FUNCIONES INTERNAS (usadas por otros módulos)
 * ============================================ */

/* Enviar comando al display */
void ssd1306_cmd(uint8_t cmd);

/* Posicionar cursor (col 0-127, page 0-3) */
void ssd1306_set_pos(uint8_t col, uint8_t page);

/* ============================================
 * FUNCIONES PÚBLICAS
 * ============================================ */

/* Inicializar display 128x32 */
uint8_t ssd1306_init(void);

/* Limpiar pantalla completa */
void ssd1306_clear(void);

/* Encender display */
void ssd1306_display_on(void);

/* Apagar display */
void ssd1306_display_off(void);

#endif /* SSD1306_CORE_H */

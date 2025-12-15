/**
 * ssd1306_scroll.c - Funciones de scroll
 */

#include "ssd1306_scroll.h"
#include "../core/ssd1306_core.h"

#if SSD1306_USE_SCROLL

/* Scroll derecha */
void ssd1306_scroll_right(uint8_t start_page, uint8_t end_page, uint8_t speed) {
    ssd1306_cmd(0x26);        /* Right scroll */
    ssd1306_cmd(0x00);        /* Dummy */
    ssd1306_cmd(start_page);  /* Start page */
    ssd1306_cmd(speed);       /* Speed (0-7) */
    ssd1306_cmd(end_page);    /* End page */
    ssd1306_cmd(0x00);        /* Dummy */
    ssd1306_cmd(0xFF);        /* Dummy */
    ssd1306_cmd(0x2F);        /* Activate scroll */
}

/* Scroll izquierda */
void ssd1306_scroll_left(uint8_t start_page, uint8_t end_page, uint8_t speed) {
    ssd1306_cmd(0x27);        /* Left scroll */
    ssd1306_cmd(0x00);        /* Dummy */
    ssd1306_cmd(start_page);  /* Start page */
    ssd1306_cmd(speed);       /* Speed (0-7) */
    ssd1306_cmd(end_page);    /* End page */
    ssd1306_cmd(0x00);        /* Dummy */
    ssd1306_cmd(0xFF);        /* Dummy */
    ssd1306_cmd(0x2F);        /* Activate scroll */
}

/* Detener scroll */
void ssd1306_scroll_stop(void) {
    ssd1306_cmd(0x2E);
}

#endif /* SSD1306_USE_SCROLL */

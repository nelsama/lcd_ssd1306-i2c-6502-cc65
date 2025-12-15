/**
 * ssd1306_core.c - Funciones básicas SSD1306
 * 
 * Módulo obligatorio: init, clear, on/off, posicionamiento
 */

#include "ssd1306_core.h"
#include "../../i2c/i2c.h"

/* ============================================
 * FUNCIONES INTERNAS
 * ============================================ */

/* Enviar comando */
void ssd1306_cmd(uint8_t cmd) {
    i2c_start(SSD1306_ADDR, I2C_WRITE);
    i2c_write_byte(SSD1306_CMD);
    i2c_write_byte(cmd);
    i2c_stop();
}

/* Posicionar cursor */
void ssd1306_set_pos(uint8_t col, uint8_t page) {
    ssd1306_cmd(0xB0 + page);
    ssd1306_cmd(0x00 + (col & 0x0F));
    ssd1306_cmd(0x10 + ((col >> 4) & 0x0F));
}

/* ============================================
 * FUNCIONES PÚBLICAS
 * ============================================ */

/* Inicializar display 128x32 */
uint8_t ssd1306_init(void) {
    /* Verificar que responde */
    if (!i2c_start(SSD1306_ADDR, I2C_WRITE)) {
        i2c_stop();
        return 0;
    }
    i2c_stop();
    
    /* Secuencia de inicialización para 128x32 */
    ssd1306_cmd(0xAE); /* Display off */
    ssd1306_cmd(0xD5); /* Set clock div */
    ssd1306_cmd(0x80);
    ssd1306_cmd(0xA8); /* Set multiplex */
    ssd1306_cmd(0x1F); /* 32 líneas */
    ssd1306_cmd(0xD3); /* Set display offset */
    ssd1306_cmd(0x00);
    ssd1306_cmd(0x40); /* Set start line */
    ssd1306_cmd(0x8D); /* Charge pump */
    ssd1306_cmd(0x14);
    ssd1306_cmd(0x20); /* Memory mode */
    ssd1306_cmd(0x00); /* Horizontal */
    ssd1306_cmd(0xA1); /* Seg remap */
    ssd1306_cmd(0xC8); /* COM scan dec */
    ssd1306_cmd(0xDA); /* Set COM pins */
    ssd1306_cmd(0x02);
    ssd1306_cmd(0x81); /* Set contrast */
    ssd1306_cmd(0x8F);
    ssd1306_cmd(0xD9); /* Set precharge */
    ssd1306_cmd(0xF1);
    ssd1306_cmd(0xDB); /* Set VCOMH */
    ssd1306_cmd(0x40);
    ssd1306_cmd(0xA4); /* Display RAM */
    ssd1306_cmd(0xA6); /* Normal display */
    ssd1306_cmd(0xAF); /* Display on */
    
    return 1;
}

/* Limpiar pantalla */
void ssd1306_clear(void) {
    uint8_t page, col;
    
    for (page = 0; page < SSD1306_PAGES; page++) {
        ssd1306_set_pos(0, page);
        
        i2c_start(SSD1306_ADDR, I2C_WRITE);
        i2c_write_byte(SSD1306_DATA);
        for (col = 0; col < SSD1306_WIDTH; col++) {
            i2c_write_byte(0x00);
        }
        i2c_stop();
    }
}

/* Encender display */
void ssd1306_display_on(void) {
    ssd1306_cmd(0xAF);
}

/* Apagar display */
void ssd1306_display_off(void) {
    ssd1306_cmd(0xAE);
}

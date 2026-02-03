# Driver SSD1306 Modular para 6502

Driver optimizado para displays OLED SSD1306 128x32 en sistemas 6502 embebidos (cc65).

## Características

| Módulo | Descripción | ROM | RAM |
|--------|-------------|-----|-----|
| **Core** | Init, clear, on/off | ~0.5 KB | 0 |
| **Text** | Fuente 5x7, ASCII 32-126 | ~1 KB | 0 |
| **Numbers** | Decimal, hex 8/16 bits | ~0.5 KB | 0 |
| **BigFont** | Caracteres 15x15 px (optimizado) | ~1.8 KB | 0 |
| **Graphics** | Líneas, rectángulos, iconos | ~0.5 KB | 0 |
| **Scroll** | Scroll horizontal | ~0.2 KB | 0 |
| **Control** | Contraste, inversión | ~0.2 KB | 0 |
| **Framebuffer** | Píxeles individuales | ~1.5 KB | 512-640 B |

### Optimizaciones BigFont (v1.1)

El módulo BigFont ha sido optimizado para 6502:
- **Tablas precalculadas** `div10[]` y `mod10[]` evitan divisiones costosas
- **Máscaras de bit** precalculadas eliminan shifts variables
- **Constantes** calculadas en tiempo de compilación
- Ahorro: ~300 bytes de código vs versión anterior

---

## Instalación

```bash
cd tu_proyecto/libs
git clone https://github.com/nelsama/lcd_ssd1306-i2c-6502-cc65.git ssd1306
```

---

## Configuración Rápida

Edita `ssd1306_config.h` y cambia `1` (activado) o `0` (desactivado):

### Opción 1: Editar directamente en la librería

Modifica `ssd1306_config.h` en el directorio de la librería.

### Opción 2: Config local en tu proyecto (Recomendado)

Crea un archivo `ssd1306_config.h` en tu proyecto que sobrescriba los valores:

```
tu_proyecto/
├── config/
│   └── ssd1306/
│       └── ssd1306_config.h   ← Tu configuración
├── libs/
│   └── ssd1306/               ← Librería sin modificar
└── Makefile
```

**En tu Makefile**, incluye config antes que libs:
```makefile
INCLUDES = -Iconfig -Ilibs
```

**Tu `config/ssd1306/ssd1306_config.h`:**
```c
#ifndef SSD1306_CONFIG_H
#define SSD1306_CONFIG_H

#define SSD1306_USE_CORE            1
#define SSD1306_USE_TEXT            1
#define SSD1306_USE_FONT_5X7_FULL   1
// Solo define lo que necesitas, el resto usa valores por defecto (0)

#endif
```

La librería usa `#ifndef` para cada define, permitiendo que tu archivo local tenga prioridad.

---

## Ejemplos de Configuración

### Ejemplo 1: Solo Reloj BigFont (~2 KB ROM)

```c
#define SSD1306_USE_CORE            1
#define SSD1306_USE_BIGNUM          1
#define SSD1306_BIGNUM_LETTERS      0   // Solo dígitos

#define SSD1306_USE_TEXT            0
#define SSD1306_USE_NUMBERS         0
#define SSD1306_USE_GRAPHICS        0
#define SSD1306_USE_SCROLL          0
#define SSD1306_USE_CONTROL         0
#define SSD1306_USE_FRAMEBUFFER     0
```

### Ejemplo 2: Texto y Números (~2.5 KB ROM)

```c
#define SSD1306_USE_CORE            1
#define SSD1306_USE_FONT_5X7_FULL   1
#define SSD1306_USE_TEXT            1
#define SSD1306_USE_NUMBERS         1
#define SSD1306_USE_NUMBERS_HEX     1

#define SSD1306_USE_BIGNUM          0
#define SSD1306_USE_GRAPHICS        0
#define SSD1306_USE_SCROLL          0
#define SSD1306_USE_CONTROL         0
#define SSD1306_USE_FRAMEBUFFER     0
```

### Ejemplo 3: Solo Framebuffer (~2.5 KB ROM, 640 B RAM)

Configuración mínima para demos gráficos sin texto:

```c
#define SSD1306_USE_CORE            1
#define SSD1306_USE_FRAMEBUFFER     1
#define SSD1306_USE_FB_LINE         1
#define SSD1306_USE_FB_CIRCLE       1
#define SSD1306_USE_FB_FILL         1
#define SSD1306_USE_FB_PLOT         1
#define SSD1306_USE_FB_SPRITE       1

#define SSD1306_USE_FONT_5X7_FULL   0
#define SSD1306_USE_TEXT            0
#define SSD1306_USE_NUMBERS         0
#define SSD1306_USE_BIGNUM          0
#define SSD1306_USE_GRAPHICS        0
#define SSD1306_USE_SCROLL          0
#define SSD1306_USE_CONTROL         0
```

### Ejemplo 4: Monitor con Gráfica (~3.5 KB ROM, 640 B RAM)

```c
#define SSD1306_USE_CORE            1
#define SSD1306_USE_FONT_5X7_FULL   1
#define SSD1306_USE_TEXT            1
#define SSD1306_USE_NUMBERS         1
#define SSD1306_USE_FRAMEBUFFER     1
#define SSD1306_USE_FB_PLOT         1   // +128 B RAM

#define SSD1306_USE_BIGNUM          0
#define SSD1306_USE_FB_LINE         0
#define SSD1306_USE_FB_CIRCLE       0
```

### Ejemplo 5: Todo Activado (~6 KB ROM, 640 B RAM)

```c
#define SSD1306_USE_CORE            1
#define SSD1306_USE_FONT_5X7_FULL   1
#define SSD1306_USE_TEXT            1
#define SSD1306_USE_TEXT_INV        1
#define SSD1306_USE_NUMBERS         1
#define SSD1306_USE_NUMBERS_HEX     1
#define SSD1306_USE_BIGNUM          1
#define SSD1306_BIGNUM_LETTERS      1
#define SSD1306_USE_GRAPHICS        1
#define SSD1306_USE_RECT            1
#define SSD1306_USE_PROGRESS        1
#define SSD1306_USE_ICONS           1
#define SSD1306_USE_SCROLL          1
#define SSD1306_USE_CONTROL         1
#define SSD1306_USE_FRAMEBUFFER     1
#define SSD1306_USE_FB_LINE         1
#define SSD1306_USE_FB_CIRCLE       1
#define SSD1306_USE_FB_FILL         1
#define SSD1306_USE_FB_PLOT         1
#define SSD1306_USE_FB_SPRITE       1
```

---

## Uso Básico

```c
#include "ssd1306/ssd1306.h"

int main(void) {
    i2c_init();
    
    if (!ssd1306_init()) {
        // Error: display no detectado
        return 1;
    }
    
    ssd1306_clear();
    ssd1306_text(0, 0, "Hola Mundo!");
    
    return 0;
}
```

---

## Jerarquía de Módulos

```
                         ┌──────────┐
                         │   CORE   │  ← Siempre requerido
                         └────┬─────┘
                              │
    ┌───────┬───────┬─────────┼─────────┬───────┬───────┐
    ▼       ▼       ▼         ▼         ▼       ▼       ▼
┌───────┐┌───────┐┌───────┐┌───────┐┌───────┐┌───────┐┌───────────┐
│CONTROL││SCROLL ││GRAPHIC││BIGNUM ││FONT5x7││       ││FRAMEBUFFER│
└───────┘└───────┘└───┬───┘└───┬───┘└───┬───┘│       │└─────┬─────┘
                      │        │        │    │       │      │
                ┌─────┴─────┐  │   ┌────┴────┤       │ ┌────┴────┐
                ▼           ▼  │   ▼         ▼       │ ▼         ▼
           ┌────────┐┌─────────┐│┌──────┐┌───────┐   │┌───────┐┌───────┐
           │  RECT  ││PROGRESS ││ TEXT ││NUMBERS│   ││FB_LINE││FB_CIRC│
           └────────┘└─────────┘│└──────┘└───┬───┘   │└───────┘└───┬───┘
                                │            │       │             │
                           ┌────┴────┐  ┌────┴────┐  │        ┌────┴────┐
                           │ BIGNUM  │  │NUMBERS  │  │        │ FB_FILL │
                           │ LETTERS │  │  HEX    │  │        └─────────┘
                           └─────────┘  └─────────┘  │
                                                     │
                                              ┌──────┴──────┐
                                              ▼             ▼
                                         ┌────────┐   ┌──────────┐
                                         │FB_PLOT │   │FB_SPRITE │
                                         └────────┘   └──────────┘
```

### Tabla de Dependencias

| Módulo | Requiere | Submódulos Opcionales |
|--------|----------|----------------------|
| CORE | - | - |
| CONTROL | CORE | - |
| SCROLL | CORE | - |
| GRAPHICS | CORE | RECT, PROGRESS, ICONS |
| FONT_5X7 | - | - |
| TEXT | CORE, FONT_5X7 | TEXT_INV |
| NUMBERS | CORE, TEXT | NUMBERS_HEX |
| BIGNUM | CORE | BIGNUM_LETTERS |
| FRAMEBUFFER | CORE | FB_LINE, FB_CIRCLE, FB_FILL, FB_PLOT, FB_SPRITE |

---

## Referencia de Funciones

### Core

```c
uint8_t ssd1306_init(void);                        // Inicializar (1=OK, 0=error)
void ssd1306_clear(void);                          // Limpiar pantalla
void ssd1306_display_on(void);                     // Encender
void ssd1306_display_off(void);                    // Apagar (bajo consumo)
void ssd1306_set_pos(uint8_t col, uint8_t page);   // Posicionar cursor
```

---

### Texto (TEXT)

Requiere: `FONT_5X7_FULL=1`, `TEXT=1`

```c
void ssd1306_char(uint8_t col, uint8_t page, char c);
void ssd1306_text(uint8_t col, uint8_t page, const char *str);
void ssd1306_text_inv(uint8_t col, uint8_t page, const char *str);  // Invertido
```

| Parámetro | Rango | Descripción |
|-----------|-------|-------------|
| col | 0-127 | Columna en píxeles |
| page | 0-3 | Fila (cada página = 8 px alto) |

**Ejemplo:**
```c
ssd1306_text(0, 0, "Linea 1");
ssd1306_text(0, 1, "Linea 2");
ssd1306_text(0, 2, "Linea 3");
ssd1306_text(0, 3, "Linea 4");

// Texto invertido (resaltado)
ssd1306_text(0, 0, "Estado: ");
ssd1306_text_inv(48, 0, "ALERTA");
```

---

### Números (NUMBERS)

Requiere: `TEXT=1`, `NUMBERS=1`

```c
void ssd1306_number(uint8_t col, uint8_t page, uint16_t num);
void ssd1306_number_signed(uint8_t col, uint8_t page, int16_t num);
void ssd1306_hex8(uint8_t col, uint8_t page, uint8_t num);   // Requiere NUMBERS_HEX
void ssd1306_hex16(uint8_t col, uint8_t page, uint16_t num); // Requiere NUMBERS_HEX
```

**Ejemplo:**
```c
ssd1306_text(0, 0, "Valor: ");
ssd1306_number(42, 0, 12345);       // 12345

ssd1306_text(0, 1, "Temp: ");
ssd1306_number_signed(36, 1, -15);  // -15

ssd1306_text(0, 2, "Reg: ");
ssd1306_hex8(30, 2, 0xAB);          // AB

ssd1306_text(0, 3, "Addr: ");
ssd1306_hex16(36, 3, 0xCAFE);       // CAFE
```

---

### BigFont (BIGNUM)

Caracteres grandes de 15x15 píxeles (5x5 bloques de 3x3 px).
Soporta: `0-9`, `A-Z` (con BIGNUM_LETTERS), `:`, `-`, `.`, espacio.

Requiere: `BIGNUM=1`

```c
// Posiciones verticales
#define BIGFONT_ROW1    0   // Fila superior
#define BIGFONT_ROW2    2   // Fila inferior
#define BIGFONT_CENTER  1   // Centrado (1 fila)

// Funciones
void ssd1306_bigchar(uint8_t x, uint8_t page, char c);
void ssd1306_bigtext(uint8_t x, uint8_t page, const char *str);
void ssd1306_bigtext_center(const char *str);

// Funciones de reloj
void ssd1306_bignum_time(uint8_t x, uint8_t page, uint8_t h, uint8_t m);
void ssd1306_bignum_time_centered(uint8_t h, uint8_t m);
void ssd1306_bignum_time_full(uint8_t h, uint8_t m, uint8_t s);
```

**Ejemplo - Reloj:**
```c
// HH:MM centrado
ssd1306_bignum_time_centered(12, 34);

// HH:MM:SS (usa 2 filas)
ssd1306_bignum_time_full(12, 34, 56);

// Texto grande
ssd1306_bigtext_center("HOLA");

// Dos líneas
ssd1306_bigtext(20, BIGFONT_ROW1, "CPU");
ssd1306_bigtext(4, BIGFONT_ROW2, "6502");
```

---

### Gráficos (GRAPHICS)

Requiere: `GRAPHICS=1`

```c
void ssd1306_hline(uint8_t x, uint8_t page, uint8_t width, uint8_t pattern);
void ssd1306_fill_page(uint8_t page, uint8_t pattern);
void ssd1306_rect(uint8_t x, uint8_t page, uint8_t w, uint8_t h_pages);      // RECT=1
void ssd1306_progress_bar(uint8_t x, uint8_t page, uint8_t w, uint8_t pct);  // PROGRESS=1
void ssd1306_icon8(uint8_t col, uint8_t page, const uint8_t *icon);          // ICONS=1
void ssd1306_clear_area(uint8_t col, uint8_t page, uint8_t width);
```

**Ejemplo:**
```c
// Líneas
ssd1306_hline(0, 1, 128, 0xFF);   // Sólida
ssd1306_hline(0, 2, 128, 0xAA);   // Punteada

// Barra de progreso
for (uint8_t i = 0; i <= 100; i += 5) {
    ssd1306_progress_bar(0, 2, 128, i);
    delay(100);
}

// Icono 8x8
static const uint8_t icon_heart[8] = {
    0x0C, 0x1E, 0x3E, 0x7C, 0x7C, 0x3E, 0x1E, 0x0C
};
ssd1306_icon8(60, 1, icon_heart);
```

---

### Scroll (SCROLL)

Requiere: `SCROLL=1`

```c
void ssd1306_scroll_left(uint8_t start_page, uint8_t end_page, uint8_t speed);
void ssd1306_scroll_right(uint8_t start_page, uint8_t end_page, uint8_t speed);
void ssd1306_scroll_stop(void);
```

| Parámetro | Rango | Descripción |
|-----------|-------|-------------|
| start_page | 0-3 | Página inicial |
| end_page | 0-3 | Página final |
| speed | 0-7 | 0=rápido, 7=lento |

**Ejemplo:**
```c
ssd1306_text(0, 0, "  MENSAJE LARGO  ");
ssd1306_scroll_left(0, 0, 5);  // Solo línea 0
delay(5000);
ssd1306_scroll_stop();
```

---

### Control (CONTROL)

Requiere: `CONTROL=1`

```c
void ssd1306_set_contrast(uint8_t contrast);  // 0-255 (default: 0x8F)
void ssd1306_invert(uint8_t invert);          // 0=normal, 1=invertido
void ssd1306_flip_h(uint8_t flip);            // Voltear horizontal
void ssd1306_flip_v(uint8_t flip);            // Voltear vertical
```

**Ejemplo:**
```c
// Fade out
for (uint8_t i = 255; i > 0; i -= 10) {
    ssd1306_set_contrast(i);
    delay(50);
}

// Parpadeo invertido
for (uint8_t i = 0; i < 6; i++) {
    ssd1306_invert(i & 1);
    delay(200);
}
ssd1306_invert(0);

// Rotar 180°
ssd1306_flip_h(1);
ssd1306_flip_v(1);
```

---

### Framebuffer (FRAMEBUFFER)

Requiere: `FRAMEBUFFER=1` (512 bytes RAM)

El framebuffer mantiene una copia de la pantalla en RAM, permitiendo manipular píxeles individuales.

> **⚠️ IMPORTANTE - Configuración de Memoria:**  
> El 6502 usa las direcciones `$0100-$01FF` para su stack de hardware (JSR/RTS/PHA/PLA/IRQ).  
> **La RAM del proyecto debe comenzar en `$0200` o superior** para evitar conflictos.
>
> Ejemplo en `fpga.cfg`:
> ```
> RAM: start = $0200, size = $3C00, fill = yes, fillval = $00;
> ```
> Si la RAM comienza en `$0100`, las funciones de framebuffer (especialmente `fb_flush()`) pueden sobrescribir el stack y causar cuelgues.

#### Funciones Básicas

```c
void fb_init(void);                    // Inicializar (llenar con 0)
void fb_clear(void);                   // Limpiar buffer
void fb_fill(void);                    // Llenar todo
void fb_flush(void);                   // Enviar al display (¡IMPORTANTE!)

void fb_set_pixel(uint8_t x, uint8_t y);     // Encender píxel
void fb_clear_pixel(uint8_t x, uint8_t y);   // Apagar píxel
void fb_toggle_pixel(uint8_t x, uint8_t y);  // Toggle (XOR)
uint8_t fb_get_pixel(uint8_t x, uint8_t y);  // Leer (0 o 1)
```

**Ejemplo:**
```c
fb_init();
fb_set_pixel(64, 16);  // Centro
fb_flush();            // ¡Enviar al display!
```

#### Líneas (FB_LINE)

```c
void fb_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);  // Diagonal (Bresenham)
void fb_hline(uint8_t x, uint8_t y, uint8_t width);            // Horizontal rápida
void fb_vline(uint8_t x, uint8_t y, uint8_t height);           // Vertical rápida
void fb_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);      // Rectángulo (solo borde)
```

> **Nota técnica:** `fb_line()` usa el algoritmo de Bresenham con variables internas `int16_t` para evitar overflow en líneas largas (>127 píxeles de diferencia).

**Ejemplo:**
```c
fb_clear();
fb_line(0, 0, 127, 31);     // Diagonal esquina a esquina
fb_line(0, 31, 127, 0);     // Diagonal cruzada (X)
fb_rect(10, 2, 108, 28);    // Marco rectangular
fb_flush();
```

#### Círculos (FB_CIRCLE)

```c
void fb_circle(int8_t cx, int8_t cy, uint8_t r);          // Solo borde
void fb_circle_filled(int8_t cx, int8_t cy, uint8_t r);   // Relleno (FB_FILL)
```

**Ejemplo:**
```c
fb_clear();
fb_circle(64, 16, 15);          // Círculo grande
fb_circle_filled(64, 16, 5);    // Centro relleno
fb_flush();
```

#### Rectángulos Rellenos (FB_FILL)

```c
void fb_rect_filled(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
```

#### Gráficas de Datos (FB_PLOT)

Requiere 128 bytes adicionales de RAM.

```c
void fb_plot(const uint8_t *data, uint8_t count, uint8_t x_offset);
void fb_plot_scaled(const uint8_t *data, uint8_t count, uint8_t min, uint8_t max);
void fb_plot_scroll(uint8_t value, uint8_t min, uint8_t max);
```

**Ejemplo - Gráfica en tiempo real:**
```c
// Añadir nueva lectura y scroll automático
while (1) {
    uint8_t temp = leer_temperatura();
    fb_plot_scroll(temp, 0, 50);  // Rango 0-50°C
    fb_flush();
    delay(1000);
}
```

#### Sprites (FB_SPRITE)

```c
void fb_sprite(uint8_t x, uint8_t y, const uint8_t *data, uint8_t w, uint8_t h);
void fb_sprite_xor(uint8_t x, uint8_t y, const uint8_t *data, uint8_t w, uint8_t h);
void fb_sprite_clear(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
```

---

## Estructura de Archivos

```
ssd1306/
├── ssd1306.h               # Header maestro (incluir este)
├── ssd1306_config.h        # Configuración de módulos
├── core/
│   ├── ssd1306_core.c      # Init, clear, on/off
│   ├── ssd1306_core.h
│   ├── ssd1306_control.c   # Contraste, inversión
│   └── ssd1306_control.h
├── fonts/
│   └── font_5x7_full.c     # Fuente ASCII 32-126
├── text/
│   ├── ssd1306_text.c
│   └── ssd1306_text.h
├── numbers/
│   ├── ssd1306_numbers.c
│   └── ssd1306_numbers.h
├── bignum/
│   ├── ssd1306_bigfont.c   # Caracteres 15x15
│   └── ssd1306_bigfont.h
├── graphics/
│   ├── ssd1306_graphics.c
│   └── ssd1306_graphics.h
├── scroll/
│   ├── ssd1306_scroll.c
│   └── ssd1306_scroll.h
└── framebuffer/
    ├── ssd1306_framebuffer.c
    └── ssd1306_framebuffer.h
```

---

## Hardware Soportado

| Parámetro | Valor |
|-----------|-------|
| Display | SSD1306 OLED |
| Resolución | 128x32 (también 128x64) |
| Interfaz | I2C |
| Dirección | 0x3C (o 0x3D) |
| Compilador | cc65 |
| CPU | 6502 |

---

## Ejemplo Completo: Reloj

```c
#include "ssd1306/ssd1306.h"

void mostrar_reloj(uint8_t h, uint8_t m, uint8_t s) {
    static uint8_t last_s = 255;
    
    if (s != last_s) {
        ssd1306_bignum_time_full(h, m, s);
        last_s = s;
    }
}

int main(void) {
    uint8_t h = 12, m = 0, s = 0;
    
    i2c_init();
    ssd1306_init();
    ssd1306_clear();
    
    while (1) {
        mostrar_reloj(h, m, s);
        
        delay(1000);
        if (++s >= 60) { s = 0; m++; }
        if (m >= 60) { m = 0; h++; }
        if (h >= 24) { h = 0; }
    }
}
```

---

## 💖 Apóyame

Si disfrutas de este proyecto, considera apoyarme:

[![Support me on Ko-fi](https://img.shields.io/badge/Ko--fi-Apóyame-FF5E5B?logo=kofi&logoColor=white&style=for-the-badge)](https://ko-fi.com/nelsonfigueroa2k)

---

## Licencia

MIT License - Libre para uso personal y comercial.

---

## Changelog

### v1.1.0 (2025-01)
- **Fix crítico:** Corregido overflow en `fb_line()` - Variables Bresenham cambiadas de `int8_t` a `int16_t` para soportar líneas de ancho completo (127 píxeles).
- **Documentación:** Añadida advertencia sobre configuración de RAM vs stack de hardware del 6502.
- **Documentación:** Añadido Ejemplo 3 "Solo Framebuffer" en sección de configuración.

### v1.0.0
- Lanzamiento inicial con todos los módulos.

---

## Autor

Driver desarrollado para CPU 6502 en FPGA Tang Nano 9K.

Repositorio: https://github.com/nelsama/lcd_ssd1306-i2c-6502-cc65

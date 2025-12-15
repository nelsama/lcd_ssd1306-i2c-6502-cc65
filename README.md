# Driver SSD1306 Modular para 6502

Driver optimizado para displays OLED SSD1306 128x32 en sistemas 6502 embebidos.

## Características

- **Modular**: Activa solo lo que necesitas, reduce tamaño de ROM
- **BigFont**: Caracteres grandes 15x15 píxeles (5x5 bloques de 3x3 px)
- **Texto 5x7**: Fuente pequeña ASCII completa
- **Gráficos**: Líneas, rectángulos, barra de progreso, iconos
- **Control**: Contraste, inversión, scroll

## Estructura de Archivos

```
libs/ssd1306/
├── ssd1306.h              # Header maestro (incluir este)
├── ssd1306_config.h       # Configuración de módulos
├── core/
│   ├── ssd1306_core.c/h   # Init, clear, on/off
│   └── ssd1306_control.c/h # Contraste, inversión
├── fonts/
│   └── font_5x7_full.c    # Fuente ASCII 32-126
├── text/
│   └── ssd1306_text.c/h   # Funciones de texto
├── numbers/
│   └── ssd1306_numbers.c/h # Números decimal/hex
├── bignum/
│   └── ssd1306_bigfont.c/h # Caracteres grandes
├── graphics/
│   └── ssd1306_graphics.c/h # Líneas, rectángulos
├── scroll/
│   └── ssd1306_scroll.c/h # Scroll horizontal
└── framebuffer/
    └── ssd1306_framebuffer.c/h # Píxeles individuales (512B RAM)
```

## Jerarquía y Dependencias

```
                    ┌─────────────────┐
                    │      CORE       │  ← Siempre requerido
                    │  init, clear    │
                    └────────┬────────┘
                             │
   ┌─────────────────────────┼─────────────────────────┐
   │            │            │            │            │
   ▼            ▼            ▼            ▼            ▼
┌────────┐ ┌────────┐ ┌──────────┐ ┌──────────┐ ┌─────────────┐
│CONTROL │ │ SCROLL │ │ GRAPHICS │ │  BIGNUM  │ │ FRAMEBUFFER │
│contrast│ │left,rgt│ │  hline   │ │  15x15   │ │  512B RAM   │
└────────┘ └────────┘ └────┬─────┘ └────┬─────┘ └──────┬──────┘
                           │            │              │
                     ┌─────┴─────┐      │    ┌─────────┼─────────┐
                     ▼           ▼      │    ▼         ▼         ▼
                ┌────────┐ ┌─────────┐  │ ┌───────┐ ┌───────┐ ┌──────┐
                │  RECT  │ │PROGRESS │  │ │FB_LINE│ │FB_CIRC│ │FB_PLT│
                └────────┘ └─────────┘  │ └───────┘ └───────┘ └──────┘
                                        │                │
                                        │          ┌─────┴─────┐
                                        │          ▼           ▼
                                        │     ┌────────┐ ┌──────────┐
                                        │     │FB_FILL │ │FB_SPRITE │
                                        │     └────────┘ └──────────┘
                                        │
                                   ┌────┴────┐
                                   │ BIGNUM  │
                                   │ LETTERS │
                                   └─────────┘

        ┌─────────────────┐
        │    FONT_5X7     │  ← Datos de fuente
        └────────┬────────┘
                 │
        ┌────────┴────────┐
        ▼                 ▼
┌───────────────┐  ┌───────────────┐
│     TEXT      │  │    NUMBERS    │
│  char, text   │  │  number, hex  │
└───────────────┘  └───────┬───────┘
                           │
                      ┌────┴────┐
                      ▼         
               ┌────────────┐
               │NUMBERS_HEX │
               └────────────┘
```

### Tabla de Dependencias

| Módulo | Requiere | Opcional |
|--------|----------|----------|
| **CORE** | - | - |
| **CONTROL** | CORE | - |
| **SCROLL** | CORE | - |
| **GRAPHICS** | CORE | RECT, PROGRESS |
| **FONT_5X7** | - | - |
| **TEXT** | CORE, FONT_5X7 | - |
| **NUMBERS** | CORE, TEXT | NUMBERS_HEX |
| **BIGNUM** | CORE | BIGNUM_LETTERS |
| **FRAMEBUFFER** | CORE | FB_LINE, FB_CIRCLE, FB_FILL, FB_PLOT, FB_SPRITE |

### Configuraciones Típicas

```
Solo reloj:        CORE ──► BIGNUM
Texto básico:      CORE ──► FONT_5X7 ──► TEXT
Monitor de datos:  CORE ──► FONT_5X7 ──► TEXT ──► NUMBERS ──► NUMBERS_HEX
Interfaz gráfica:  CORE ──► GRAPHICS ──► RECT ──► PROGRESS
Juegos/Gráficas:   CORE ──► FRAMEBUFFER ──► FB_LINE ──► FB_CIRCLE
Todo:              Todos los módulos activados
```

---

## Configuración

Edita `ssd1306_config.h` para activar/desactivar módulos:

```c
/* Solo BigFont para reloj (~2KB) */
#define SSD1306_USE_CORE            1
#define SSD1306_USE_BIGNUM          1
#define SSD1306_USE_TEXT            0  // Desactivado
#define SSD1306_USE_GRAPHICS        0  // Desactivado
```

## Uso Básico

```c
#include "ssd1306/ssd1306.h"

int main(void) {
    i2c_init();
    
    if (!ssd1306_init()) {
        // Error: display no encontrado
        return 1;
    }
    
    ssd1306_clear();
    ssd1306_text(0, 0, "Hola Mundo!");
    
    return 0;
}
```

---

## Referencia de Funciones

### Core (siempre disponible)

```c
uint8_t ssd1306_init(void);      // Inicializar display, retorna 1=OK, 0=error
void ssd1306_clear(void);        // Limpiar pantalla
void ssd1306_display_on(void);   // Encender display
void ssd1306_display_off(void);  // Apagar display
void ssd1306_set_pos(uint8_t col, uint8_t page);  // Posicionar cursor
```

**Ejemplo:**
```c
ssd1306_init();
ssd1306_clear();
ssd1306_display_off();  // Modo bajo consumo
delay(1000);
ssd1306_display_on();
```

---

### Texto Pequeño (5x7 píxeles)

Requiere: `SSD1306_USE_TEXT=1`, `SSD1306_USE_FONT_5X7_FULL=1`

```c
void ssd1306_char(uint8_t col, uint8_t page, char c);
void ssd1306_text(uint8_t col, uint8_t page, const char *str);
void ssd1306_text_inv(uint8_t col, uint8_t page, const char *str);  // Invertido
```

- **col**: Columna en píxeles (0-127)
- **page**: Página (0-3, cada página = 8 píxeles de alto)
- Cada carácter ocupa 6 píxeles de ancho

**Ejemplo:**
```c
// 4 líneas de texto
ssd1306_text(0, 0, "Linea 1");
ssd1306_text(0, 1, "Linea 2");
ssd1306_text(0, 2, "Linea 3");
ssd1306_text(0, 3, "Linea 4");

// Texto invertido (resaltado)
ssd1306_text(0, 0, "Normal: ");
ssd1306_text_inv(48, 0, "ALERTA");

// Caracteres especiales
ssd1306_text(0, 2, "!@#$%^&*()[]{}");
```

---

### Números

Requiere: `SSD1306_USE_NUMBERS=1` y/o `SSD1306_USE_NUMBERS_HEX=1`

```c
void ssd1306_number(uint8_t col, uint8_t page, uint16_t num);
void ssd1306_number_signed(uint8_t col, uint8_t page, int16_t num);
void ssd1306_hex8(uint8_t col, uint8_t page, uint8_t num);
void ssd1306_hex16(uint8_t col, uint8_t page, uint16_t num);
```

**Ejemplo:**
```c
// Números decimales
ssd1306_text(0, 0, "Valor: ");
ssd1306_number(42, 0, 12345);      // Muestra: 12345

ssd1306_text(0, 1, "Temp: ");
ssd1306_number_signed(36, 1, -15); // Muestra: -15

// Números hexadecimales
ssd1306_text(0, 2, "Reg: ");
ssd1306_hex8(30, 2, 0xAB);         // Muestra: AB

ssd1306_text(0, 3, "Addr: ");
ssd1306_hex16(36, 3, 0xCAFE);      // Muestra: CAFE
```

---

### BigFont (15x15 píxeles)

Requiere: `SSD1306_USE_BIGNUM=1`

Caracteres de 5x5 bloques, cada bloque de 3x3 píxeles = 15x15 px total.
Soporta: 0-9, A-Z, `:`, `-`, `.`, espacio

```c
// Constantes de posición vertical
#define BIGFONT_ROW1    0   // Línea superior
#define BIGFONT_ROW2    2   // Línea inferior  
#define BIGFONT_CENTER  1   // Centrado vertical (1 línea)

// Funciones
void ssd1306_bigchar(uint8_t x, uint8_t page, char c);
void ssd1306_bigtext(uint8_t x, uint8_t page, const char *str);
void ssd1306_bigtext_center(const char *str);  // Centrado H+V

// Funciones de reloj
void ssd1306_bignum_time(uint8_t x, uint8_t page, uint8_t h, uint8_t m);
void ssd1306_bignum_time_centered(uint8_t h, uint8_t m);      // HH:MM centrado
void ssd1306_bignum_time_full(uint8_t h, uint8_t m, uint8_t s); // HH:MM:SS
```

**Ejemplo - Texto grande:**
```c
// Texto centrado horizontal y verticalmente
ssd1306_clear();
ssd1306_bigtext_center("HOLA");

// Dos líneas
ssd1306_clear();
ssd1306_bigtext(20, BIGFONT_ROW1, "CPU");
ssd1306_bigtext(4, BIGFONT_ROW2, "6502");

// Una línea centrada
ssd1306_clear();
ssd1306_bigtext(0, BIGFONT_CENTER, "12345");
```

**Ejemplo - Reloj:**
```c
// Reloj HH:MM centrado
ssd1306_clear();
ssd1306_bignum_time_centered(12, 34);  // Muestra: 12:34

// Reloj HH:MM:SS (usa las 2 líneas)
ssd1306_clear();
ssd1306_bignum_time_full(12, 34, 56);  // Muestra: 12:34:56

// Reloj con fecha
ssd1306_clear();
ssd1306_bignum_time(4, BIGFONT_ROW1, 12, 34);
ssd1306_bigtext(4, BIGFONT_ROW2, "14DIC");
```

**Capacidad por línea:**
- Máximo 8 caracteres por línea (128 ÷ 16 = 8)
- HH:MM = 5 caracteres, cabe centrado
- HH:MM:SS = 8 caracteres, línea completa

---

### Gráficos

Requiere: `SSD1306_USE_GRAPHICS=1`, `SSD1306_USE_RECT=1`, etc.

```c
void ssd1306_hline(uint8_t x, uint8_t page, uint8_t width, uint8_t pattern);
void ssd1306_fill_page(uint8_t page, uint8_t pattern);
void ssd1306_rect(uint8_t x, uint8_t page, uint8_t width, uint8_t height_pages);
void ssd1306_progress_bar(uint8_t x, uint8_t page, uint8_t width, uint8_t percent);
void ssd1306_icon8(uint8_t col, uint8_t page, const uint8_t *icon);
void ssd1306_clear_area(uint8_t col, uint8_t page, uint8_t width);
```

**Ejemplo - Líneas:**
```c
// Línea sólida
ssd1306_hline(0, 1, 128, 0xFF);

// Línea punteada
ssd1306_hline(0, 2, 128, 0xAA);  // 10101010

// Llenar página completa
ssd1306_fill_page(3, 0x55);      // Patrón alternado
```

**Ejemplo - Rectángulos:**
```c
// 3 rectángulos
ssd1306_rect(0, 2, 30, 2);   // x=0, 30px ancho, 2 páginas alto
ssd1306_rect(40, 2, 30, 2);
ssd1306_rect(80, 2, 30, 2);
```

**Ejemplo - Barra de progreso:**
```c
ssd1306_text(0, 0, "Cargando...");

// Animación de carga
for (uint8_t i = 0; i <= 100; i += 5) {
    ssd1306_progress_bar(0, 2, 128, i);
    delay(100);
}
```

**Ejemplo - Iconos 8x8:**
```c
// Definir icono (8 bytes, columnas de arriba a abajo)
static const uint8_t icon_heart[8] = {
    0x0C, 0x1E, 0x3E, 0x7C, 0x7C, 0x3E, 0x1E, 0x0C
};

static const uint8_t icon_check[8] = {
    0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00
};

// Mostrar iconos
ssd1306_icon8(10, 1, icon_heart);
ssd1306_icon8(30, 1, icon_check);

// Limpiar área
ssd1306_clear_area(10, 1, 8);  // Borra el corazón
```

---

### Scroll

Requiere: `SSD1306_USE_SCROLL=1`

```c
void ssd1306_scroll_right(uint8_t start_page, uint8_t end_page, uint8_t speed);
void ssd1306_scroll_left(uint8_t start_page, uint8_t end_page, uint8_t speed);
void ssd1306_scroll_stop(void);
```

- **start_page, end_page**: Rango de páginas (0-3)
- **speed**: 0=más rápido, 7=más lento

**Ejemplo:**
```c
ssd1306_text(0, 0, "  SCROLL DEMO  ");
ssd1306_text(0, 1, "<<< IZQUIERDA");
ssd1306_text(0, 2, ">>> DERECHA");

// Scroll izquierda, todas las páginas, velocidad media
ssd1306_scroll_left(0, 3, 5);
delay(3000);
ssd1306_scroll_stop();

// Scroll derecha
ssd1306_scroll_right(0, 3, 5);
delay(3000);
ssd1306_scroll_stop();
```

---

### Control de Display

Requiere: `SSD1306_USE_CONTROL=1`

```c
void ssd1306_set_contrast(uint8_t contrast);  // 0-255
void ssd1306_invert(uint8_t invert);          // 0=normal, 1=invertido
void ssd1306_flip_h(uint8_t flip);            // Flip horizontal
void ssd1306_flip_v(uint8_t flip);            // Flip vertical
```

**Ejemplo:**
```c
// Fade out / fade in
for (uint8_t i = 255; i > 0; i -= 10) {
    ssd1306_set_contrast(i);
    delay(50);
}
for (uint8_t i = 0; i < 255; i += 10) {
    ssd1306_set_contrast(i);
    delay(50);
}
ssd1306_set_contrast(0x8F);  // Valor por defecto

// Parpadeo con inversión
for (uint8_t i = 0; i < 6; i++) {
    ssd1306_invert(i & 1);
    delay(200);
}
ssd1306_invert(0);

// Rotar display 180°
ssd1306_flip_h(1);
ssd1306_flip_v(1);
```

---

## Tamaños de ROM por Configuración

| Configuración | Módulos | Tamaño aprox. |
|---------------|---------|---------------|
| Mínimo reloj | CORE + BIGNUM | ~2 KB |
| Texto básico | CORE + FONT + TEXT | ~1.5 KB |
| Texto + números | + NUMBERS | ~2 KB |
| BigFont completo | + BIGNUM_LETTERS | ~2.5 KB |
| Con gráficos | + GRAPHICS + RECT + PROGRESS | ~3.5 KB |
| Todo activado | Todos los módulos | ~6 KB |

---

## Hardware Soportado

- **Display**: SSD1306 OLED 128x32 (también 128x64 ajustando `SSD1306_HEIGHT`)
- **Interfaz**: I2C
- **Direcciones**: 0x3C (por defecto) o 0x3D
- **CPU**: 6502 (cc65), adaptable a otros

---

## Ejemplo Completo: Reloj

```c
#include "ssd1306/ssd1306.h"

// En ssd1306_config.h:
// SSD1306_USE_CORE=1, SSD1306_USE_BIGNUM=1, resto=0

void mostrar_reloj(uint8_t h, uint8_t m, uint8_t s) {
    static uint8_t last_s = 255;
    
    // Solo actualizar si cambió
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
        
        // Incrementar tiempo (simulado)
        delay(1000);
        if (++s >= 60) { s = 0; m++; }
        if (m >= 60) { m = 0; h++; }
        if (h >= 24) { h = 0; }
    }
}
```

---

## Framebuffer (Manipulación de Píxeles)

Requiere: `SSD1306_USE_FRAMEBUFFER=1` (512 bytes RAM)

El framebuffer mantiene una copia de la pantalla en RAM, permitiendo:
- Set/clear píxeles individuales
- Leer estado de píxeles
- Líneas diagonales, círculos, formas geométricas
- Sprites con transparencia/XOR
- Gráficas de datos en tiempo real

### Configuración Modular

```c
/* En ssd1306_config.h */
#define SSD1306_USE_FRAMEBUFFER     1   /* Básico: 512B RAM */
#define SSD1306_USE_FB_LINE         1   /* Líneas diagonales */
#define SSD1306_USE_FB_CIRCLE       1   /* Círculos */
#define SSD1306_USE_FB_FILL         1   /* Rellenos */
#define SSD1306_USE_FB_PLOT         1   /* Gráficas (+128B RAM) */
#define SSD1306_USE_FB_SPRITE       1   /* Sprites XOR */
```

### Funciones Básicas

```c
fb_init();              // Inicializar buffer (llenar con 0)
fb_clear();             // Limpiar buffer
fb_fill();              // Llenar todo de blanco
fb_flush();             // Enviar buffer al display

fb_set_pixel(x, y);     // Encender píxel
fb_clear_pixel(x, y);   // Apagar píxel
fb_toggle_pixel(x, y);  // Toggle (XOR)
fb_get_pixel(x, y);     // Leer estado (0 o 1)
```

**Ejemplo - Dibujar y mostrar:**
```c
fb_init();
fb_set_pixel(64, 16);   // Centro de la pantalla
fb_set_pixel(65, 16);
fb_set_pixel(64, 17);
fb_flush();             // ¡Importante! Enviar al display
```

### Líneas (FB_LINE)

```c
fb_line(x0, y0, x1, y1);  // Línea diagonal (Bresenham)
fb_hline(x, y, width);    // Línea horizontal (optimizada)
fb_vline(x, y, height);   // Línea vertical (optimizada)
fb_rect(x, y, w, h);      // Rectángulo (solo borde)
```

**Ejemplo - Triángulo:**
```c
fb_clear();
fb_line(64, 0, 0, 31);    // Lado izquierdo
fb_line(64, 0, 127, 31);  // Lado derecho
fb_hline(0, 31, 128);     // Base
fb_flush();
```

### Círculos (FB_CIRCLE)

```c
fb_circle(cx, cy, r);          // Círculo (solo borde)
fb_circle_filled(cx, cy, r);   // Círculo relleno (requiere FB_FILL)
```

**Ejemplo:**
```c
fb_clear();
fb_circle(64, 16, 15);         // Círculo grande
fb_circle_filled(64, 16, 5);   // Punto central relleno
fb_flush();
```

### Rectángulos Rellenos (FB_FILL)

```c
fb_rect_filled(x, y, w, h);    // Rectángulo sólido
```

### Gráficas de Datos (FB_PLOT)

```c
// Array de datos (valores 0-31 para altura)
fb_plot(data, count, x_offset);

// Con escala automática
fb_plot_scaled(data, count, min_val, max_val);

// Gráfica con scroll (añade punto, desplaza)
fb_plot_scroll(new_value, min_val, max_val);
```

**Ejemplo - Gráfica de temperatura:**
```c
uint8_t temps[128];
uint8_t idx = 0;

void add_temperature(uint8_t temp) {
    fb_plot_scroll(temp, 0, 50);  // Rango 0-50°C
    fb_flush();
}

// En el loop principal:
while (1) {
    uint8_t t = leer_temperatura();
    add_temperature(t);
    delay(1000);
}
```

### Sprites (FB_SPRITE)

```c
// Sprite con OR (píxeles se suman)
fb_sprite(x, y, sprite_data, width, height);

// Sprite con XOR (para cursor parpadeante)
fb_sprite_xor(x, y, sprite_data, width, height);

// Borrar área de sprite
fb_sprite_clear(x, y, width, height);
```

**Ejemplo - Cursor parpadeante:**
```c
static const uint8_t cursor[8] = {
    0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF
};

// Parpadeo con XOR
while (1) {
    fb_sprite_xor(60, 12, cursor, 8, 8);
    fb_flush();
    delay(500);
}
```

### Ejemplo Completo - Mini Pong

```c
#define BALL_SIZE 4
#define PADDLE_H  8

int ball_x = 64, ball_y = 16;
int ball_dx = 1, ball_dy = 1;
int paddle_y = 12;
uint8_t score = 0;

void game_loop(void) {
    while (1) {
        fb_clear();
        
        // Dibujar pelota
        fb_rect_filled(ball_x, ball_y, BALL_SIZE, BALL_SIZE);
        
        // Dibujar paleta
        fb_rect_filled(2, paddle_y, 2, PADDLE_H);
        
        // Mover pelota
        ball_x += ball_dx;
        ball_y += ball_dy;
        
        // Rebotes
        if (ball_y <= 0 || ball_y >= 28) ball_dy = -ball_dy;
        if (ball_x >= 124) ball_dx = -ball_dx;
        
        // Colisión paleta
        if (ball_x <= 4 && ball_y >= paddle_y && ball_y < paddle_y + PADDLE_H) {
            ball_dx = -ball_dx;
            score++;
        }
        
        // Game over
        if (ball_x < 0) {
            fb_clear();
            // Mostrar puntuación...
            break;
        }
        
        // Control paleta
        if (boton_arriba() && paddle_y > 0) paddle_y--;
        if (boton_abajo() && paddle_y < 24) paddle_y++;
        
        fb_flush();
        delay(30);
    }
}
```

---

## Tamaños de ROM por Configuración

| Configuración | Módulos | ROM | RAM |
|---------------|---------|-----|-----|
| Mínimo reloj | CORE + BIGNUM | ~2 KB | 0 |
| Texto básico | CORE + FONT + TEXT | ~1.5 KB | 0 |
| Con números | + NUMBERS | ~2 KB | 0 |
| Con gráficos | + GRAPHICS | ~3.5 KB | 0 |
| Framebuffer básico | + FRAMEBUFFER | +0.5 KB | +512B |
| FB + líneas | + FB_LINE | +0.3 KB | +512B |
| FB + círculos | + FB_CIRCLE | +0.4 KB | +512B |
| FB + gráficas | + FB_PLOT | +0.5 KB | +640B |
| Todo activado | Todos | ~8 KB | ~640B |

---

## Licencia

MIT License - Libre para uso personal y comercial.

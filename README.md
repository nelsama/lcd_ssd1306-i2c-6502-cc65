# Libreria SSD1306 OLED para 6502 (cc65)

Driver para displays OLED SSD1306 (128x32) para microprocesador 6502 usando compilador cc65.
Sin framebuffer - escribe directamente al display para ahorrar RAM.

## Caracteristicas

- **Texto**: Fuente ASCII completa (32-126), mayusculas y minusculas
- **Numeros**: Decimal, con signo, hexadecimal (8/16 bits)
- **Graficos**: Lineas, rectangulos, barras de progreso, iconos
- **Efectos**: Control de contraste, inversion, scroll horizontal
- **RAM minima**: No requiere framebuffer

## Hardware

- Display: SSD1306 OLED 128x32 (0.91")
- Interfaz: I2C en direccion 0x3C
- Probado en: FPGA Tang Nano 9K con soft-core 6502

## Dependencias

- [i2c-6502-cc65](https://github.com/nelsama/i2c-6502-cc65) - Libreria I2C

## Instalacion

Clonar en la carpeta `libs/` de tu proyecto:

```bash
cd libs
git clone https://github.com/nelsama/lcd_ssd1306-i2c-6502-cc65.git ssd1306
```

## Integracion con Makefile

Agregar estas lineas a tu Makefile:

```makefile
# Directorios
SSD1306_DIR = libs/ssd1306
I2C_DIR = libs/i2c

# Includes
INCLUDES = -I$(SSD1306_DIR) -I$(I2C_DIR)

# Objeto SSD1306
SSD1306_OBJ = build/ssd1306.o

# Regla de compilacion
$(SSD1306_OBJ): $(SSD1306_DIR)/ssd1306.c
$(CC65) $(CFLAGS) -I$(I2C_DIR) -I$(SSD1306_DIR) -o build/ssd1306.s $<
$(CA65) -t none -o $@ build/ssd1306.s

# Agregar a OBJS
OBJS = ... $(SSD1306_OBJ) ...
```

## Archivos

- `ssd1306.h` - Header con prototipos de funciones
- `ssd1306.c` - Implementacion

## Referencia de API

### Funciones Basicas

| Funcion | Descripcion |
|---------|-------------|
| `ssd1306_init()` | Inicializar display (retorna 1=OK, 0=error) |
| `ssd1306_clear()` | Limpiar pantalla |
| `ssd1306_display_on()` | Encender display |
| `ssd1306_display_off()` | Apagar display |

### Control de Display

| Funcion | Descripcion |
|---------|-------------|
| `ssd1306_set_contrast(0-255)` | Ajustar brillo |
| `ssd1306_invert(0/1)` | Invertir colores |
| `ssd1306_flip_h(0/1)` | Voltear horizontal |
| `ssd1306_flip_v(0/1)` | Voltear vertical |

### Scroll

| Funcion | Descripcion |
|---------|-------------|
| `ssd1306_scroll_right(inicio, fin, velocidad)` | Scroll derecha |
| `ssd1306_scroll_left(inicio, fin, velocidad)` | Scroll izquierda |
| `ssd1306_scroll_stop()` | Detener scroll |

### Texto

| Funcion | Descripcion |
|---------|-------------|
| `ssd1306_text(col, pagina, str)` | Escribir texto |
| `ssd1306_text_inv(col, pagina, str)` | Escribir texto invertido |
| `ssd1306_char(col, pagina, c)` | Escribir un caracter |

### Numeros

| Funcion | Descripcion |
|---------|-------------|
| `ssd1306_number(col, pagina, num)` | Decimal sin signo (0-65535) |
| `ssd1306_number_signed(col, pagina, num)` | Decimal con signo |
| `ssd1306_hex8(col, pagina, num)` | Hexadecimal 8 bits |
| `ssd1306_hex16(col, pagina, num)` | Hexadecimal 16 bits |

### Graficos

| Funcion | Descripcion |
|---------|-------------|
| `ssd1306_hline(x, pagina, ancho, patron)` | Linea horizontal |
| `ssd1306_fill_page(pagina, patron)` | Llenar pagina con patron |
| `ssd1306_rect(x, pagina, ancho, alto)` | Rectangulo |
| `ssd1306_progress_bar(x, pagina, ancho, porcentaje)` | Barra de progreso |
| `ssd1306_icon8(col, pagina, icono)` | Dibujar icono 8x8 |
| `ssd1306_clear_area(col, pagina, ancho)` | Limpiar area |

## Ejemplo

```c
#include "ssd1306.h"
#include "i2c.h"

int main(void) {
    i2c_init();
    
    if (ssd1306_init()) {
        ssd1306_clear();
        ssd1306_text(0, 0, "Hola 6502!");
        ssd1306_text(0, 1, "SSD1306 OLED");
        ssd1306_number(0, 2, 12345);
        ssd1306_progress_bar(0, 3, 100, 75);
    }
    
    while(1);
    return 0;
}
```

## Coordenadas

- **col**: Posicion columna (0-127 pixeles)
- **pagina**: Pagina de fila (0-3, cada pagina tiene 8 pixeles de alto)
- Display es 128x32 = 128 columnas x 4 paginas

## Uso de Memoria

- Codigo: ~2.5KB
- RAM: Minima (sin framebuffer)
- Fuente: 475 bytes (95 caracteres x 5 bytes)

## Licencia

MIT License

## Autor

Nelson Madhavan (nelsama)

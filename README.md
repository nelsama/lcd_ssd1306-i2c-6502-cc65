# SSD1306 OLED Library for 6502 (cc65)

Driver for SSD1306 OLED displays (128x32) for 6502 microprocessor using cc65 compiler.
No framebuffer - writes directly to display to save RAM.

## Features

- **Text**: Full ASCII font (32-126), uppercase and lowercase
- **Numbers**: Decimal, signed, hexadecimal (8/16 bit)
- **Graphics**: Lines, rectangles, progress bars, icons
- **Effects**: Contrast control, invert, horizontal scroll
- **Minimal RAM**: No framebuffer needed

## Hardware

- Display: SSD1306 OLED 128x32 (0.91")
- Interface: I2C at address 0x3C
- Tested on: Tang Nano 9K FPGA with 6502 soft-core

## Dependencies

- [i2c-6502-cc65](https://github.com/nelsama/i2c-6502-cc65) - I2C library

## Files

- `ssd1306.h` - Header with function prototypes
- `ssd1306.c` - Implementation

## API Reference

### Basic Functions

| Function | Description |
|----------|-------------|
| `ssd1306_init()` | Initialize display (returns 1=OK, 0=error) |
| `ssd1306_clear()` | Clear screen |
| `ssd1306_display_on()` | Turn display on |
| `ssd1306_display_off()` | Turn display off |

### Display Control

| Function | Description |
|----------|-------------|
| `ssd1306_set_contrast(0-255)` | Set brightness |
| `ssd1306_invert(0/1)` | Invert colors |
| `ssd1306_flip_h(0/1)` | Horizontal flip |
| `ssd1306_flip_v(0/1)` | Vertical flip |

### Scroll

| Function | Description |
|----------|-------------|
| `ssd1306_scroll_right(start, end, speed)` | Scroll right |
| `ssd1306_scroll_left(start, end, speed)` | Scroll left |
| `ssd1306_scroll_stop()` | Stop scrolling |

### Text

| Function | Description |
|----------|-------------|
| `ssd1306_text(col, page, str)` | Write text |
| `ssd1306_text_inv(col, page, str)` | Write inverted text |
| `ssd1306_char(col, page, c)` | Write single character |

### Numbers

| Function | Description |
|----------|-------------|
| `ssd1306_number(col, page, num)` | Decimal unsigned (0-65535) |
| `ssd1306_number_signed(col, page, num)` | Decimal signed |
| `ssd1306_hex8(col, page, num)` | Hexadecimal 8-bit |
| `ssd1306_hex16(col, page, num)` | Hexadecimal 16-bit |

### Graphics

| Function | Description |
|----------|-------------|
| `ssd1306_hline(x, page, width, pattern)` | Horizontal line |
| `ssd1306_fill_page(page, pattern)` | Fill page with pattern |
| `ssd1306_rect(x, page, width, height)` | Rectangle |
| `ssd1306_progress_bar(x, page, width, percent)` | Progress bar |
| `ssd1306_icon8(col, page, icon)` | Draw 8x8 icon |
| `ssd1306_clear_area(col, page, width)` | Clear area |

## Example

```c
#include "ssd1306.h"
#include "i2c.h"

int main(void) {
    i2c_init();
    
    if (ssd1306_init()) {
        ssd1306_clear();
        ssd1306_text(0, 0, "Hello 6502!");
        ssd1306_text(0, 1, "SSD1306 OLED");
        ssd1306_number(0, 2, 12345);
        ssd1306_progress_bar(0, 3, 100, 75);
    }
    
    while(1);
    return 0;
}
```

## Coordinates

- **col**: Column position (0-127 pixels)
- **page**: Row page (0-3, each page is 8 pixels high)
- Display is 128x32 = 128 columns x 4 pages

## Memory Usage

- Code: ~2.5KB
- RAM: Minimal (no framebuffer)
- Font: 475 bytes (95 characters x 5 bytes)

## License

MIT License

## Author

Nelson Madhavan (nelsama)

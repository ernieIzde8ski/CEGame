#ifndef EI_SHORTCUTS
#define EI_SHORTCUTS

#include <c++/cmath>
#include <c++/cstring>
#include <graphx.h>

// general purpose
#define recolor(x, y, c) gfx_vbuffer[x][y] = c
#define valid_coord(x, y) \
    (x>=0 && x <= GFX_LCD_HEIGHT) && (y >= 0 && y <= GFX_LCD_WIDTH)
#define copy_row(n, pRow) memcpy(gfx_vbuffer[n], pRow, GFX_LCD_WIDTH)
#define screen(color) memset(gfx_vbuffer, color, sizeof gfx_vbuffer)
#define screen_clear screen(0)

// circle rendering stuffs
const int CENTER_X = GFX_LCD_HEIGHT / 2;
const int CENTER_Y = GFX_LCD_WIDTH / 2;
#define HALF_HYPOTENUSE sqrt(pow(CENTER_X, 2.0) + pow(CENTER_Y, 2.0))

#define centered_circle(color, radius) circle( \
    color, radius, CENTER_X, CENTER_Y \
)

#endif

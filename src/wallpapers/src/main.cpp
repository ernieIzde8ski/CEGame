#include <c++/cmath>
#include <c++/cstdio>
#include <c++/cstring>
#include <ti/getkey.h>
#include <ti/screen.h>

#include <graphx.h>
#include <debug.h>

#include "colors.h"
#include "shortcuts.cpp"

// Failed attempt at a checkers grid
void draw_buttons(unsigned block_size) {
    unsigned size2 = block_size * 2;
    int row1[GFX_LCD_WIDTH];
    int row2[GFX_LCD_WIDTH];

    for (int y = 0; y < GFX_LCD_WIDTH; ++y) {
        bool enabled = (y % size2) < block_size;
        row1[y] = enabled ? C_BLACK : C_WHITE;
        row2[y] = enabled ? C_WHITE : C_BLACK;
    }

    for (int x = 0; x < GFX_LCD_HEIGHT; ++x) {
        int* p_row = &((x % size2) < block_size ? row1 : row2)[0];
        copy_row(x, p_row);
    }
}

// Messed around, and this happened.
void draw_diagonal_stripes(unsigned block_size) {
    for (int x = 0; x < GFX_LCD_HEIGHT; ++x) {
        for (int y = 0; y < GFX_LCD_WIDTH; y++) {
            recolor(x, y, block_size * (x + y) % 256);
        }
    }
}

// helper for draw_centered_circles()
void circle(int color, int r, int h, int k) {
    // for ease of my brain, here are the rules:
    // - assume that for each Y coordinate, there is one or two
    //   corresponding X coordinates with a pixel marked
    // - floor everything for simplicity
    // - there are less X coordinates than Y coordinates, as X
    //   corresponds to height (row) and Y corresponds to length

    const double r_sq = pow(r, 2.0);

    // color in topmost and bottommost points
    recolor(h - r, k, color);
    recolor(h + r, k, color);
    // topmost and bottommost coordinates
    int x = (h - r) + 1;
    int x_1 = (h + r) - 1;
    for (; x <= h; ++x & --x_1) {
        double offset = sqrt(r_sq - pow(x - h, 2));
        // I spent a lot of time here trying to figure out how to
        // make the dots more connected, but I gave up eventually.
        int y_0 = k - offset;
        int y_1 = k + offset;
        // if one corner of the "rectangle" is valid, all are
        if (valid_coord(x, y_0)) {
            recolor(x, y_0, color);
            recolor(x, y_1, color);
            recolor(x_1, y_0, color);
            recolor(x_1, y_1, color);
        };
    }
}

// Draws circles around the center of the calculator
void draw_centered_circles(unsigned block_size) {
    // make the whole thing black
    screen_clear;

    int displacement = block_size * 3;
    for (int radius = displacement / 2; radius < HALF_HYPOTENUSE; radius += displacement) {
        centered_circle(C_MAGENTA, radius);
    }
}

void event_loop() {
    using PATTERN = void(*)(unsigned int);
    PATTERN patterns[] = {
        draw_buttons,
        draw_diagonal_stripes,
        draw_centered_circles,
    };
    int pattern = 0;
    int patterns_total = sizeof(patterns) / sizeof(patterns[0]);

    uint16_t key_press = k_Up;
    unsigned block_size = 0;

    do {
        // handle keypresses
        // Up & Plus increment block size
        // Down & Minus decrement block size
        // Left and Right move between pattern choice
        switch (key_press) {
        case k_Up:
        case k_Add:
            ++block_size;
            break;
        case k_Down:
        case k_Sub:
            if (block_size > 1)
                block_size -= 1;
            break;
        case k_Right:
            if ((++pattern) == patterns_total) pattern = 0;
            block_size = 1;
            break;
        case k_Left:
            if ((--pattern) == -1) pattern = patterns_total - 1;
            block_size = 1;
            break;
        default:
            return;
        }

        // actually draw the pattern
        patterns[pattern](block_size);
    } while ((key_press = os_GetKey()));
}

int main(void) {

    gfx_Begin();
    event_loop();
    gfx_End();

    return 0;
}

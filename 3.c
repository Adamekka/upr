//! Funkce

#include "drawing.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define eprintf(...) (void)fprintf(stderr, __VA_ARGS__)

typedef enum {
    DASHED_LINE = 0,
    STAIRS = 1,
    FLOWER = 2,
    MEADOW = 3,
    ANIMATION = 4,
} Drawing;

typedef enum {
    RED,
    GREEN,
    BLUE,
    WHITE,
    YELLOW,
    BLACK,
} Color;

void dashed_line(uint16_t len, Color color);
void stairs(uint16_t count, Color color);
void flower(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void meadow(uint16_t cols, uint16_t rows, uint16_t width, uint16_t height);
void animation();

int main() {
    clear_screen();

    Drawing drawing;
    scanf("%u", &drawing);

    switch (drawing) {
        case DASHED_LINE: {
            dashed_line(20, WHITE);
            printf("\n\n");
            dashed_line(20, GREEN);
            break;
        }

        case STAIRS: {
            stairs(20, WHITE);
            stairs(10, GREEN);
            break;
        }

        case FLOWER: {
            flower(0, 20, 20, 20);
            flower(20, 20, 15, 15);
            break;
        }

        case MEADOW: meadow(5, 2, 10, 10); break;
        case ANIMATION: animation(); break;
        default: eprintf("ERROR: Bad drawing value\n"); return -1;
    }

    end_drawing();
}

void set_color(Color color) {
    switch (color) {
        case RED: set_red_color(); break;
        case GREEN: set_green_color(); break;
        case BLUE: set_blue_color(); break;
        case WHITE: set_white_color(); break;
        case YELLOW: set_yellow_color(); break;
        case BLACK: set_black_color(); break;
    }
}

void dashed_line(uint16_t len, Color color) {
    for (uint16_t j = 0; j < len; j++) {
        set_color(color);
        putchar(' ');
        reset_color();
        putchar(' ');
    }
}

void stairs(uint16_t count, Color color) {
    set_color(color);
    move_to(0, count);

    for (uint16_t i = 0; i < count; i++) {
        printf("   ");
        move_down();
        move_left();
    }
}

void flower(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    set_green_color();

    move_to(y + height, x + width / 2);
    draw_pixel();
    move_up();

    srand((uint32_t)time(NULL));

    for (uint16_t y = 0; y < height / 2;) {
        const enum Direction {
            LEFT = 0,
            RIGHT = 1,
            UP = 2,
        } direction
            = (enum Direction)rand() % 3;

        switch (direction) {
            case LEFT: move_left(); break;
            case RIGHT: move_right(); break;

            case UP: {
                move_up();
                y++;
                break;
            }

            default: assert(false);
        }

        draw_pixel();
    }

    const uint16_t RED_HEIGHT = (uint16_t)(10 * height * 0.025);
    const uint16_t RED_WIDTH = (uint16_t)(10 * width * 0.05);
    const uint16_t YELLOW_HEIGHT = (uint16_t)(5 * height * 0.025);
    const uint16_t YELLOW_WIDTH = (uint16_t)(5 * width * 0.05);

    set_red_color();

    for (uint16_t i = 0; i < RED_WIDTH / 2; i++)
        move_left();

    for (uint16_t i = 0; i < RED_HEIGHT / 2; i++)
        move_up();

    for (uint16_t y = 0; y < RED_HEIGHT; y++) {
        for (uint16_t x = 0; x < RED_WIDTH; x++) {
            if (!((x == 0 && y == 0) || (x == 0 && y == RED_HEIGHT - 1)
                  || (x == RED_WIDTH - 1 && y == 0)
                  || (x == RED_WIDTH - 1 && y == RED_HEIGHT - 1)))
                draw_pixel();

            move_right();
        }

        move_down();

        for (uint16_t i = 0; i < RED_WIDTH; i++)
            move_left();
    }

    set_yellow_color();

    for (uint16_t i = 0; i < (YELLOW_WIDTH / 2); i++)
        move_right();

    for (uint16_t i = 0; i < (YELLOW_HEIGHT * 1.5); i++)
        move_up();

    for (uint16_t y = 0; y < YELLOW_HEIGHT; y++) {
        for (uint16_t x = 0; x < YELLOW_WIDTH; x++) {
            if (!((x == 0 && y == 0) || (x == 0 && y == YELLOW_HEIGHT - 1)
                  || (x == YELLOW_WIDTH - 1 && y == 0)
                  || (x == YELLOW_WIDTH - 1 && y == YELLOW_HEIGHT - 1)))
                draw_pixel();

            move_right();
        }

        move_down();

        for (uint8_t i = 0; i < YELLOW_WIDTH; i++)
            move_left();
    }
}

void meadow(uint16_t cols, uint16_t rows, uint16_t width, uint16_t height) {
    for (uint16_t row = 0; row < rows; row++)
        for (uint16_t col = 0; col < cols; col++)
            flower(width * col, height * row, width, height);
}

void draw_ball(uint16_t x, uint16_t y) {
    clear_screen();
    set_black_color();

    move_to(y, x);

    const uint8_t BALL_SIZE = 4;

    for (uint8_t y = 0; y < BALL_SIZE; y++) {
        for (uint8_t x = 0; x < BALL_SIZE; x++) {
            if (!((x == 0 && y == 0) || (x == 0 && y == BALL_SIZE - 1)
                  || (x == BALL_SIZE - 1 && y == 0)
                  || (x == BALL_SIZE - 1 && y == BALL_SIZE - 1)))
                draw_pixel();

            move_right();
        }

        move_down();

        for (uint8_t i = 0; i < BALL_SIZE; i++)
            move_left();
    }

    animate();
}

void animation() {
    uint16_t x = 0;
    uint16_t y = 0;

    bool down = true;

    while (true) {
        draw_ball(x, y);

        x++;

        down ? y++ : y--;

        if ((down && y == canvas_height) || (!down && y == 0))
            down = !down;
    }
}

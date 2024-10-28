//! Dynamická alokace paměti

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TURTLE_COUNT 3

#define M(arr, i, j) ((arr)[(i) * (cols) + (j)])
#define eprintf(...) (void)fprintf(stderr, __VA_ARGS__)

typedef enum {
    R = 'r',
    L = 'l',
    M = 'm',
    O = 'o',
    F = 'f',
    X = 'x',
} Input;

typedef enum {
    TOP,
    RIGHT,
    DOWN,
    LEFT,
} Direction;

#define DIRECTION_SIZE (LEFT + 1)

typedef struct {
    uint16_t r, c;
} Pos;

typedef struct {
    bool o; // o symbol
} Cell;

typedef struct {
    Pos pos;
    Direction direction;
} Turtle;

int main() {
    uint16_t rows, cols;
    scanf("%hu%hu", &rows, &cols);

    // Calloc will set everything to 0, so it's all false
    Cell* arr = (Cell*)calloc(rows * cols, sizeof(*arr));

    Turtle turtles[MAX_TURTLE_COUNT] = {(Turtle){{0, 0}, RIGHT}};
    uint8_t turtle_count = 1;

    while (true) {
        char input_;
        scanf(" %c", &input_);

        const Input input = input_ == 'r' || input_ == 'l' || input_ == 'm'
                                 || input_ == 'o' || input_ == 'f'
                                 || input_ == 'x'
                              ? (Input)input_
                              : '\0';

        if (input == '\0') {
            eprintf("ERROR: Bad input");
            free(arr);
            arr = NULL;
            return -1;
        }

        if (input == X)
            break;

        if (input == F) {
            if (turtle_count < MAX_TURTLE_COUNT)
                turtles[turtle_count++] = (Turtle){{0, 0}, RIGHT};

            // If more turtles are added, ignore them
            continue;
        }

        for (uint8_t i = 0; i < turtle_count; i++) {
            Turtle* turtle = &turtles[i];

            if (input == O) {
                M(arr, turtle->pos.r, turtle->pos.c).o
                    = !M(arr, turtle->pos.r, turtle->pos.c).o;
#ifdef DEBUG
                printf(
                    "Turtle %u O: %u, %u\n", i, turtle->pos.r, turtle->pos.c
                );
#endif
                continue;
            }

            if (input == R) {
                turtle->direction = (turtle->direction + 1) % DIRECTION_SIZE;
#ifdef DEBUG
                printf("Turtle %u direction: %u\n", i, turtle->direction);
#endif
                continue;
            }

            if (input == L) {
                turtle->direction
                    = (turtle->direction + DIRECTION_SIZE - 1) % DIRECTION_SIZE;
#ifdef DEBUG
                printf("Turtle %u direction: %u\n", i, turtle->direction);
#endif
                continue;
            }

            // This code executes when MOVE
            assert(input == M);

            switch (turtle->direction) {
                case TOP: {
                    turtle->pos.r = (uint16_t)(turtle->pos.r + rows - 1) % rows;
                    break;
                }

                case RIGHT: {
                    turtle->pos.c = (uint16_t)(turtle->pos.c + 1) % cols;
                    break;
                }

                case DOWN: {
                    turtle->pos.r = (uint16_t)(turtle->pos.r + 1) % rows;
                    break;
                }

                case LEFT: {
                    turtle->pos.c = (uint16_t)(turtle->pos.c + cols - 1) % cols;
                    break;
                }
            }

#ifdef DEBUG
            printf("Turtle %u pos: %u, %u\n", i, turtle->pos.r, turtle->pos.c);
#endif

            assert(turtle->pos.r < rows && turtle->pos.c < cols);
        }
    }

    for (uint16_t i = 0; i < rows; ++i) {
        for (uint16_t j = 0; j < cols; ++j) {
            Cell cell = M(arr, i, j);
            char symbol = cell.o ? 'o' : '.';
            printf("%c", symbol);
        }

        puts("");
    }

    free(arr);
    arr = NULL;
}

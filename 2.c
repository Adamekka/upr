//! Cykly a podmínky

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define newl() puts("")

typedef enum {
    FILL = 0,
    BORDERS = 1,
    FILL_NUMS = 2,
    DIAGONAL1 = 3,
    DIAGONAL2 = 4,
    TRIANGLE = 5,
    T = 6,
    H = 7,
    FILL_NUMS2 = 9,
} Obrazec;

int main() {
    Obrazec obrazec;
    uint32_t a, b;
    scanf("%u%u%u", &obrazec, &a, &b);

    switch (obrazec) {
        case FILL: {
            for (uint32_t i = 0; i < b; i++) {
                for (uint32_t j = 0; j < a; j++)
                    putchar('x');

                newl();
            }
            break;
        }

        case BORDERS: {
            for (uint32_t i = 0; i < b; i++) {
                for (uint32_t j = 0; j < a; j++)
                    if (i == 0 || i == b - 1 || j == 0 || j == a - 1)
                        putchar('x');
                    else
                        putchar(' ');

                newl();
            }
            break;
        }

        case FILL_NUMS: {
            uint32_t counter = 0;
            for (uint32_t i = 0; i < b; i++) {
                for (uint32_t j = 0; j < a; j++)
                    if (i == 0 || i == b - 1 || j == 0 || j == a - 1)
                        putchar('x');
                    else {
                        printf("%u", counter);

                        if (++counter >= 10)
                            counter -= 10;
                    }

                newl();
            }
            break;
        }

        case DIAGONAL1: {
            for (uint32_t i = 0; i < a; i++) {
                for (uint32_t j = 0; j < i; j++)
                    putchar(' ');

                puts("x");
            }
            break;
        }

        case DIAGONAL2: {
            for (uint32_t i = 0; i < a; i++) {
                for (uint32_t j = 0; j < (a - i - 1); j++)
                    putchar(' ');

                putchar('x');

                for (uint32_t j = 0; j < i; j++)
                    putchar(' ');

                newl();
            }
            break;
        }

        case TRIANGLE: {
            uint32_t inside = 0;
            for (uint32_t i = 0; i < (a - 1); i++) {
                for (uint32_t j = 0; j < (a - i - 1); j++)
                    putchar(' ');

                putchar('x');

                for (uint32_t j = 0; j < inside; j++)
                    putchar(' ');

                if (i == 0) {
                    inside++;
                } else {
                    inside += 2;
                    putchar('x');
                }

                for (uint32_t j = 0; j < (a - i - 1); j++)
                    putchar(' ');

                newl();
            }

            for (uint32_t i = 0; i < (2 * a - 1); i++)
                putchar('x');

            newl();
            break;
        }

        case T: {
            assert(a % 2 == 1);

            for (uint32_t i = 0; i < a; i++)
                putchar('x');

            newl();

            for (uint32_t i = 0; i < (b - 1); i++) {
                for (uint32_t j = 0; j < (a / 2); j++)
                    putchar(' ');

                puts("x");
            }

            break;
        }

        case H: {
            assert(b % 2 == 1);

            bool legs_done = false;

        legs:
            for (uint32_t i = 0; i < ((b - 1) / 2); i++) {
                putchar('x');

                for (uint32_t j = 0; j < (a - 2); j++)
                    putchar(' ');

                puts("x");
            }

            if (legs_done)
                break;
            else
                legs_done = true;

            for (uint32_t i = 0; i < a; i++)
                putchar('x');

            newl();

            goto legs;

            assert(false); // unreachable
            break;
        }

        case FILL_NUMS2: {
            int32_t col = -1;
            for (uint32_t i = 0; i < b; i++) {
                uint32_t row = (uint32_t)col;
                for (uint32_t j = 0; j < a; j++)
                    if (i == 0 || i == b - 1 || j == 0 || j == a - 1)
                        putchar('x');
                    else {
                        printf("%u", row);

                        row += (b - 2);

                        while (row >= 10)
                            row -= 10;
                    }

                col++;
                while (col >= 10)
                    col -= 10;

                newl();
            }
            break;
        }

        default: printf("Neznamy obrazec\n"); return 0;
    }
}

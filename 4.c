//! Ukazatele

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define HISTOGRAM_BIN_COUNT 9

typedef enum {
    HORIZONTAL = 'h',
    VERTICAL = 'v',
} Direction;

static inline uint32_t max(uint32_t a, uint32_t b);
uint8_t int_char_count(uint32_t n);
uint32_t arr_max(uint32_t arr[HISTOGRAM_BIN_COUNT]);

int main() {
    char direction_;
    scanf("%c", &direction_);

    const Direction direction
        = direction_ == 'h' || direction_ == 'v' ? (Direction)direction_ : '\0';

    if (direction == '\0') {
        puts("Neplatny mod vykresleni");
        return 1;
    }

    uint32_t n, m;
    scanf("%u%u", &n, &m);

    const uint32_t M_MAX = m + 8;

    uint32_t histogram[HISTOGRAM_BIN_COUNT] = {0};
    uint32_t invalid = 0;

    for (uint32_t i = 0; i < n; i++) {
        uint32_t input;
        scanf("%u", &input);

        if (m <= input && input <= M_MAX)
            histogram[input - m]++;
        else
            invalid++;
    }

    switch (direction) {
        case HORIZONTAL: {
            const uint8_t M_MAX_CHAR_COUNT = int_char_count(M_MAX);

            for (uint32_t i = 0; i < HISTOGRAM_BIN_COUNT; i++) {
                uint32_t val = i + m;

                if (int_char_count(val) != M_MAX_CHAR_COUNT)
                    putchar(' ');

                printf("%u", val);

                if (histogram[i] > 0)
                    putchar(' ');

                for (uint32_t j = 0; j < histogram[i]; j++)
                    putchar('#');

                puts("");
            }

            if (invalid > 0) {
                printf("invalid: ");

                for (uint32_t j = 0; j < invalid; j++)
                    putchar('#');

                puts("");
            }

            break;
        }

        case VERTICAL: {
            assert(m == 1 && M_MAX == 9);

            if (n == 0)
                goto end;

            uint32_t height = max(invalid, arr_max(histogram));

            for (uint32_t y = height; y > 0; y--) {
                if (invalid >= y)
                    putchar('#');
                else
                    putchar(' ');

                for (uint32_t x = 0; x < HISTOGRAM_BIN_COUNT; x++)
                    if (histogram[x] >= y)
                        putchar('#');
                    else
                        putchar(' ');

                puts("");
            }

        end:

            putchar('i');

            for (uint32_t i = m; i <= M_MAX; i++)
                printf("%u", i);

            puts("");
            break;
        }
    }
}

static inline uint32_t max(uint32_t a, uint32_t b) { return a > b ? a : b; }

uint8_t int_char_count(uint32_t n) {
    uint8_t count = 0;

    if (n == 0)
        return 1;

    while (n != 0) {
        n /= 10;
        count++;
    }

    return count;
}

uint32_t arr_max(uint32_t arr[HISTOGRAM_BIN_COUNT]) {
    uint32_t res = 0;

    for (uint32_t i = 0; i < HISTOGRAM_BIN_COUNT; i++)
        if (arr[i] > res)
            res = arr[i];

    return res;
}

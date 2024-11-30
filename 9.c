//! Binární práce se soubory a TGA

#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LETTERS 26
#define LETTER_PATH_SIZE 50
#define INPUT_CHARS_PER_LINE 101

#define TEXT_HORIZONTAL_PADDING 20
#define SPACE_WIDTH 10
#define AVG_LETTER_WIDTH 20
#define AVG_LETTER_HEIGHT 34

uint16_t str_pixel_width(const char* str);

typedef struct __attribute__((packed)) {
    uint8_t id;
    uint8_t color_map_type;
    uint8_t image_type;
    uint16_t color_map_origin;
    uint16_t color_map_len;
    uint8_t color_map_depth;
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t depth;
    uint8_t descriptor;
} TGAHeader;

typedef struct __attribute__((packed)) {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} Pixel;

typedef struct {
    TGAHeader header;
    Pixel* pixels;
} Image;

int main(int32_t argc, char** argv) {
    assert(sizeof(TGAHeader) == 18);

    if (argc != 4) {
        puts("Wrong parameters");
        return 1;
    }

    FILE* input = fopen(argv[1], "rb");
    FILE* output = fopen(argv[2], "wb");
    const char* fonts_path = argv[3];

    Image image;

    if (input == NULL) {
        puts("Could not load image");
        return 1;
    }

    assert(output != NULL);

    fread(&image.header, sizeof(TGAHeader), 1, input);

    image.pixels = (Pixel*)malloc(
        image.header.width * image.header.height * sizeof(Pixel)
    );

    fread(
        image.pixels,
        sizeof(Pixel),
        image.header.width * image.header.height,
        input
    );

    Image letters[LETTERS];

    for (int i = 0; i < LETTERS; i++) {
        Image* letter = &letters[i];

        char path[LETTER_PATH_SIZE];
        snprintf(path, LETTER_PATH_SIZE, "%s/%c.tga", fonts_path, i + 'A');

        if (access(path, F_OK) == -1)
            return 1;

        FILE* file = fopen(path, "rb");
        fread(&letter->header, sizeof(TGAHeader), 1, file);

        letter->pixels = (Pixel*)malloc(
            letter->header.width * letter->header.height * sizeof(Pixel)
        );

        fread(
            letter->pixels,
            sizeof(Pixel),
            letter->header.width * letter->header.height,
            file
        );
    }

    char row_count_input[sizeof("1 1\n")];
    fgets(row_count_input, sizeof(row_count_input), stdin);

    assert(isdigit(row_count_input[0]));
    assert(row_count_input[1] == ' ');
    assert(isdigit(row_count_input[2]));

    const uint8_t top = (uint8_t)(row_count_input[0] - '0');
    const uint8_t bottom = (uint8_t)(row_count_input[2] - '0');

    for (uint8_t i = 0; i < (top + bottom); i++) {
        char str[INPUT_CHARS_PER_LINE];
        fgets(str, INPUT_CHARS_PER_LINE, stdin);

        uint16_t x_pos = image.header.width / 2 - str_pixel_width(str) / 2;

        for (const char* c = str; *c != '\0'; c++) {
            if (*c == ' ') {
                x_pos += SPACE_WIDTH;
                continue;
            }

            const Image* letter = &letters[toupper(*c) - 'A'];
            assert(letter != NULL);

            const uint16_t x_start = x_pos;
            x_pos += letter->header.width;

            const uint16_t y_start
                = i >= top ? image.header.height - TEXT_HORIZONTAL_PADDING
                                 - AVG_LETTER_HEIGHT * (top + bottom - i)
                           : TEXT_HORIZONTAL_PADDING + AVG_LETTER_HEIGHT * i;

            const uint16_t x_end = x_start + letter->header.width;
            const uint16_t y_end = y_start + letter->header.height;

            for (uint16_t y = y_start; y < y_end; y++) {
                for (uint16_t x = x_start; x < x_end; x++) {
                    Pixel* image_pixel
                        = &image.pixels[y * image.header.width + x];

                    const uint16_t letter_x = x - x_start;
                    const uint16_t letter_y = y - y_start;
                    const Pixel* letter_pixel
                        = &letter->pixels
                               [letter_y * letter->header.width + letter_x];

                    if (letter_pixel->r == 0 && letter_pixel->g == 0
                        && letter_pixel->b == 0)
                        continue;

                    image_pixel->r = letter_pixel->r;
                    image_pixel->g = letter_pixel->g;
                    image_pixel->b = letter_pixel->b;
                }
            }
        }
    }

    fwrite(&image.header, sizeof(TGAHeader), 1, output);
    fwrite(
        image.pixels,
        sizeof(Pixel),
        image.header.width * image.header.height,
        output
    );

    free(image.pixels);
    image.pixels = NULL;

    for (int i = 0; i < LETTERS; i++) {
        free(letters[i].pixels);
        letters[i].pixels = NULL;
    }
}

uint16_t str_pixel_width(const char* str) {
    uint16_t len = 0;

    for (const char* c = str; *c != '\0'; c++)
        if (*c == ' ')
            len += SPACE_WIDTH;
        else
            len += AVG_LETTER_WIDTH;

    return len;
}

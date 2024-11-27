//! Textová práce se soubory

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define FLAGS "io:"
#define FILE_CHARS_PER_LINE 100

int main(int32_t argc, char** argv) {
    char* input = NULL;
    char* needle = NULL;
    bool case_insensitive = false;
    char* output = NULL;

    uint16_t pos = 1;
    for (uint16_t i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            if (case_insensitive) {
                printf("Parameter -i provided multiple times\n");
                return 1;
            }

            case_insensitive = true;
        } else if (strcmp(argv[i], "-o") == 0) {
            if (output != NULL) {
                printf("Parameter -o provided multiple times\n");
                return 1;
            }

            if (i + 1 >= argc) {
                printf("Missing output path\n");
                return 1;
            }

            output = argv[++i];
        } else if (pos == 1) {
            input = argv[i];
            pos++;
        } else if (pos == 2) {
            needle = argv[i];
            pos++;
        } else {
            printf("Too many parameters provided\n");
            return 1;
        }
    }

    if (input == NULL) {
        printf("Input path not provided\n");
        return 1;
    }

    if (needle == NULL) {
        printf("Needle not provided\n");
        return 1;
    }

    FILE* input_file = fopen(input, "r");
    assert(input_file != NULL);

    FILE* output_file = stdout;

    if (output != NULL)
        output_file = fopen(output, "w");

    char line[FILE_CHARS_PER_LINE];
    while (fgets(line, FILE_CHARS_PER_LINE, input_file) != NULL)
        if (case_insensitive) {
            if (strcasestr(line, needle) != NULL)
                fprintf(output_file, "%s", line);
        } else {
            if (strstr(line, needle) != NULL)
                fprintf(output_file, "%s", line);
        }
}

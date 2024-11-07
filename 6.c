//! Práce s textem

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_CHARS_PER_LINE 51

int main() {
    char input_row_count[10];
    fgets(input_row_count, sizeof(input_row_count), stdin);
    uint16_t row_count = (uint16_t)atoi(input_row_count);

    // Used for formatting output
    bool first = true;

    for (uint16_t i = 0; i < row_count; i++) {
        char line[INPUT_CHARS_PER_LINE];

        fgets(line, INPUT_CHARS_PER_LINE, stdin);

        uint16_t initial_upper_count = 0;
        uint16_t initial_lower_count = 0;
        uint16_t initial_spaces = 0;

        uint16_t final_upper_count = 0;
        uint16_t final_lower_count = 0;

        char* tmp = line;
        while (*tmp != '\0') {
            if (*tmp == ' ')
                initial_spaces++;

            tmp++;
        }

        char res[INPUT_CHARS_PER_LINE] = "";

        char* str = strtok(line, " ");
        while (str != NULL) {
            uint16_t upper_count = 0;
            uint16_t lower_count = 0;

            char* tmp = str;
            while (*tmp != '\0') {
                if (isupper(*tmp))
                    upper_count++;
                else if (islower(*tmp))
                    lower_count++;

                tmp++;
            }

            tmp = str;
            char* write_ptr = str;
            while (*tmp != '\0') {
                if (write_ptr == str || *tmp != write_ptr[-1])
                    *write_ptr++ = *tmp;
                tmp++;
            }
            *write_ptr = '\0';

            tmp = str;
            while (*tmp != '\0') {
                if (*tmp == '\n')
                    *tmp = '\0';

                tmp++;
            }

            if (upper_count == 0) { // xxx -> XXX
                char* tmp = str;
                while (*tmp != '\0') {
                    *tmp = (char)toupper(*tmp);
                    tmp++;
                }
                final_upper_count += (uint16_t)strlen(str);
            } else if (lower_count == 0) { // XXX -> xxx
                char* tmp = str;
                while (*tmp != '\0') {
                    *tmp = (char)tolower(*tmp);
                    tmp++;
                }
                final_lower_count += (uint16_t)strlen(str);
            } else { // xXxXXx -> Xxxxxx
                char* tmp = str;
                *tmp = (char)toupper(*tmp);
                tmp++;
                while (*tmp != '\0') {
                    *tmp = (char)tolower(*tmp);
                    tmp++;
                }
                final_upper_count++;
                final_lower_count += (uint16_t)strlen(str) - 1;
                assert(strlen(str) > 0);
            }

            initial_upper_count += upper_count;
            initial_lower_count += lower_count;

            strcat(res, str);
            strcat(res, " ");
            str = strtok(NULL, " ");
        }

        uint16_t len = (uint16_t)strlen(res);
        if (len > 0) {
            tmp = &res[len - 1];
            while (tmp >= res && *tmp == ' ') {
                *tmp = '\0';
                tmp--;
            }
        }

        uint16_t final_spaces = 0;

        tmp = res;
        while (*tmp != '\0') {
            if (*tmp == ' ')
                final_spaces++;

            tmp++;
        }

        if (first)
            first = false;
        else
            printf("\n");

        printf("%s\n", res);
        printf("lowercase: %u -> %u\n", initial_lower_count, final_lower_count);
        printf("uppercase: %u -> %u\n", initial_upper_count, final_upper_count);
        printf("spaces: %u -> %u\n", initial_spaces, final_spaces);
    }
}

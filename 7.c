//! Struktury

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_CHARS_PER_LINE 101
#define STOCK_NAME_SIZE 5
#define UINT32_T_MAX_DIGITS 11
#define UINT32_T_FORMATTED_MAX_DIGITS (11 + 3)

typedef struct {
    uint16_t day_idx;
    char name[STOCK_NAME_SIZE];
    double value_start;
    double value_end;
    uint32_t transaction_count;
} Stock;

Stock* Stock_hightest_transactions(Stock s[], char name[STOCK_NAME_SIZE]);

void int_format(uint32_t n, char buffer[UINT32_T_FORMATTED_MAX_DIGITS]);

uint16_t g_input_row_count = UINT16_MAX;

int main(int32_t argc, char** argv) {
    if (argc != 3) {
        puts("Wrong parameters");
        return 1;
    }

    g_input_row_count = (uint16_t)atoi(argv[2]);

    Stock* stocks = (Stock*)malloc(g_input_row_count * sizeof(Stock));

    for (uint16_t i = 0; i < g_input_row_count; i++) {
        char line[INPUT_CHARS_PER_LINE];

        fgets(line, INPUT_CHARS_PER_LINE, stdin);

        Stock stock;

        stock.day_idx = (uint16_t)atoi(strtok(line, ","));
        strcpy(stock.name, strtok(NULL, ","));
        stock.value_start = (double)atof(strtok(NULL, ","));
        stock.value_end = (double)atof(strtok(NULL, ","));
        stock.transaction_count = (uint32_t)atoi(strtok(NULL, ","));

        assert(strtok(NULL, ",") == NULL);

        stocks[i] = stock;
    }

    printf("<html>\n"
           "<body>\n"
           "<div>\n");

    Stock* highest = Stock_hightest_transactions(stocks, argv[1]);

    if (highest != NULL) {
        printf("<h1>%s: highest volume</h1>\n", highest->name);
        printf("<div>Day: %hu</div>\n", highest->day_idx);
        printf("<div>Start price: %.2f</div>\n", highest->value_start);
        printf("<div>End price: %.2f</div>\n", highest->value_end);

        char formatted[UINT32_T_FORMATTED_MAX_DIGITS];
        int_format(highest->transaction_count, formatted);
        printf("<div>Volume: %s</div>\n", formatted);
    } else {
        printf("Ticker %s was not found\n", argv[1]);
    }

    printf("</div>\n"
           "<table>\n"
           "<thead>\n"
           "<tr><th>Day</th><th>Ticker</th><th>Start</th><th>End</th>"
           "<th>Diff</th><th>Volume</th></tr>\n"
           "</thead>\n"
           "<tbody>\n");

    for (int16_t i = (int16_t)(g_input_row_count - 1); i >= 0; i--) {
        Stock* current = &stocks[i];
        assert(current != NULL);

        printf("<tr>\n");

        if (strcmp(current->name, argv[1]) == 0) {
            printf("\t<td><b>%hu</b></td>\n", current->day_idx);
            printf("\t<td><b>%s</b></td>\n", current->name);
            printf("\t<td><b>%.2f</b></td>\n", current->value_start);
            printf("\t<td><b>%.2f</b></td>\n", current->value_end);
            printf(
                "\t<td><b>%.2f</b></td>\n",
                current->value_end - current->value_start
            );

            char formatted[UINT32_T_FORMATTED_MAX_DIGITS];
            int_format(current->transaction_count, formatted);
            printf("\t<td><b>%s</b></td>\n", formatted);
        } else {
            printf("\t<td>%hu</td>\n", current->day_idx);
            printf("\t<td>%s</td>\n", current->name);
            printf("\t<td>%.2f</td>\n", current->value_start);
            printf("\t<td>%.2f</td>\n", current->value_end);
            printf(
                "\t<td>%.2f</td>\n", current->value_end - current->value_start
            );

            char formatted[UINT32_T_FORMATTED_MAX_DIGITS];
            int_format(current->transaction_count, formatted);
            printf("\t<td>%s</td>\n", formatted);
        }

        printf("</tr>\n");
    }

    printf("</tbody>\n"
           "</table>\n"
           "</body>\n"
           "</html>\n");

    free(stocks);
    stocks = NULL;
}

Stock* Stock_hightest_transactions(Stock s[], char name[STOCK_NAME_SIZE]) {
    assert(g_input_row_count != UINT16_MAX);

    Stock* highest = NULL;

    uint16_t i = 0;

    for (; i < g_input_row_count; i++) {
        Stock* current = &s[i];

        if (strcmp(current->name, name) == 0) {
            highest = current;
            break;
        }
    }

    if (highest == NULL)
        return NULL;

    i++;

    for (; i < g_input_row_count; i++) {
        Stock* current = &s[i];

        highest
            = strcmp(current->name, name) == 0
                   && current->transaction_count > highest->transaction_count
                ? current
                : highest;
    }

    return highest;
}

void str_reverse(char* str) {
    char* end = str + strlen(str) - 1;

    while (str < end) {
        char tmp = *str;
        *str = *end;
        *end = tmp;

        str++;
        end--;
    }
}

void int_format(uint32_t n, char buffer[UINT32_T_FORMATTED_MAX_DIGITS]) {
    char tmp[UINT32_T_MAX_DIGITS];
    snprintf(tmp, UINT32_T_MAX_DIGITS, "%u", n);
    str_reverse(tmp);

    for (uint16_t i = 0, j = 0; i < UINT32_T_FORMATTED_MAX_DIGITS; i++)
        if ((i + 1) % 4 == 0 && i != 0)
            buffer[i] = '_';
        else
            buffer[i] = tmp[j++];

    str_reverse(buffer);
}

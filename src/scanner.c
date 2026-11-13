#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "patterns.h"

int is_candidate_char(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           c == '.' || c == '-' ||
           c == '_' || c == '@';
}

int add_finding(ScanResult *result, const char *type,
                int line, int column, int length)
{
    Finding *temp = realloc(
        result->items,
        (result->count + 1) * sizeof(Finding)
    );

    if (temp == NULL)
        return 0;

    result->items = temp;

    Finding *f = &result->items[result->count];

    strcpy(f->type, type);
    f->line = line;
    f->column = column;
    f->length = length;

    result->count++;

    return 1;
}

int scan_text(const char *text, size_t length, ScanResult *result)
{
    result->items = NULL;
    result->count = 0;
    result->capacity = 0;

    int line = 1;
    int column = 1;
    size_t i = 0;

    while (i < length) {

        if (is_candidate_char(text[i])) {

            size_t start = i;
            int start_column = column;

            while (i < length && is_candidate_char(text[i])) {
                i++;
                column++;
            }

            size_t n = i - start;

            while (n > 0 && text[start + n - 1] == '.')
                n--;

            const char *type = match_pattern(text + start, n);

            if (type != NULL) {
                if (!add_finding(result, type, line,
                                 start_column, (int)n)) {
                    free_scan_result(result);
                    return 0;
                }
            }
        }
        else if (text[i] == '\r' && i + 1 < length &&
                 text[i + 1] == '\n') {
            i += 2;
            line++;
            column = 1;
        }
        else if (text[i] == '\n') {
            i++;
            line++;
            column = 1;
        }
        else {
            /* Move over one UTF-8 character */
            unsigned char c = (unsigned char)text[i];

            if (c < 0x80)
                i++;
            else if ((c & 0xE0) == 0xC0)
                i += 2;
            else if ((c & 0xF0) == 0xE0)
                i += 3;
            else
                i += 4;

            if (i > length)
                i = length;

            column++;
        }
    }

    return 1;
}

void free_scan_result(ScanResult *result)
{
    free(result->items);
    result->items = NULL;
    result->count = 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "report.h"

/* Read input file */
char *read_file(const char *filename, size_t *length)
{
    FILE *file = fopen(filename, "rb");

    if (file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    rewind(file);

    char *text = malloc(*length + 1);

    if (text == NULL) {
        fclose(file);
        return NULL;
    }

    *length = fread(text, 1, *length, file);
    text[*length] = '\0';

    fclose(file);

    return text;
}

/* Write sanitized text */
int write_file(const char *filename, const char *text, size_t length)
{
    FILE *file = fopen(filename, "wb");

    if (file == NULL)
        return 0;

    fwrite(text, 1, length, file);

    fclose(file);

    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: privacy_scanner input.txt\n");
        return 1;
    }

    size_t length;

    char *text = read_file(argv[1], &length);

    if (text == NULL) {
        printf("Cannot read input file.\n");
        return 1;
    }

    ScanResult result;

    if (!scan_text(text, length, &result)) {
        printf("Scanning failed.\n");
        free(text);
        return 1;
    }

    /* Create sanitized text */
    char *sanitized = malloc(length * 2 + 1);

    if (sanitized == NULL) {
        free(text);
        free_scan_result(&result);
        return 1;
    }

    size_t i = 0;
    size_t j = 0;
    size_t finding_index = 0;

    int line = 1;
    int column = 1;

    while (i < length) {
        if (finding_index < result.count) {
            Finding f = result.items[finding_index];

            if (line == f.line && column == f.column) {
                const char *label = "[EMAIL]";

                if (strcmp(f.type, "PHONE") == 0)
                    label = "[PHONE]";
                else if (strcmp(f.type, "IP") == 0)
                    label = "[IP]";

                size_t label_length = strlen(label);

                memcpy(sanitized + j, label, label_length);
                j += label_length;

                /*
                 * Findings are ASCII-only, so column advances by
                 * the finding length.
                 */
                i += (size_t)f.length;
                column += f.length;
                finding_index++;

                continue;
            }
        }

        unsigned char c = (unsigned char)text[i];

        if (c == '\r' && i + 1 < length && text[i + 1] == '\n') {
            /* Keep CRLF exactly and count it as one line break. */
            sanitized[j++] = text[i++];
            sanitized[j++] = text[i++];
            line++;
            column = 1;
        }
        else if (c == '\n') {
            sanitized[j++] = text[i++];
            line++;
            column = 1;
        }
        else {
            /*
             * Copy one UTF-8 character and count it as one column,
             * matching scanner.c.
             */
            size_t char_len;

            if (c < 0x80)
                char_len = 1;
            else if ((c & 0xE0) == 0xC0)
                char_len = 2;
            else if ((c & 0xF0) == 0xE0)
                char_len = 3;
            else
                char_len = 4;

            if (i + char_len > length)
                char_len = length - i;

            memcpy(sanitized + j, text + i, char_len);
            i += char_len;
            j += char_len;
            column++;
        }
    }

    sanitized[j] = '\0';

    if (!write_file("sanitized.txt", sanitized, j))
        printf("Could not write sanitized.txt\n");

    if (!write_report("report.json", &result))
        printf("Could not write report.json\n");

    printf("Detected: %zu\n", result.count);

    free(text);
    free(sanitized);
    free_scan_result(&result);

    return 0;
}
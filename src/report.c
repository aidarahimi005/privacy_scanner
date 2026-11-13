#include <stdio.h>
#include "report.h"

int write_report(const char *filename, const ScanResult *result)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
        return 0;

    fprintf(file, "{\n");
    fprintf(file, "  \"total_detected\": %zu,\n", result->count);
    fprintf(file, "  \"findings\": [");

    for (size_t i = 0; i < result->count; i++) {
        Finding f = result->items[i];

        fprintf(file,
            "%s\n    {\n"
            "      \"type\": \"%s\",\n"
            "      \"line\": %d,\n"
            "      \"column\": %d,\n"
            "      \"length\": %d\n"
            "    }",
            i == 0 ? "" : ",",
            f.type,
            f.line,
            f.column,
            f.length
        );
    }

    if (result->count > 0)
        fprintf(file, "\n  ");

    fprintf(file, "]\n}\n");

    return fclose(file) == 0;
}
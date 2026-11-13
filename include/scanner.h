#ifndef SCANNER_H
#define SCANNER_H

#include <stddef.h>

typedef struct {
    char type[6];       /* EMAIL, PHONE or IP */
    int line;
    int column;
    int length;
} Finding;

typedef struct {
    Finding *items;
    size_t count;
    size_t capacity;
} ScanResult;

/* Scan input text and create findings */
int scan_text(const char *text, size_t length, ScanResult *result);

/* Free memory used by scan result */
void free_scan_result(ScanResult *result);

#endif
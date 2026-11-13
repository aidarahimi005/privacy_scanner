#ifndef PATTERNS_H
#define PATTERNS_H

#include <stddef.h>

int is_valid_email(const char *text, size_t length);

int is_valid_phone(const char *text, size_t length);

int is_valid_ipv4(const char *text, size_t length);

const char *match_pattern(const char *text, size_t length);

#endif

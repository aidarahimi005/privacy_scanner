#include <stddef.h>
#include "patterns.h"

int is_letter(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

int is_alphanumeric(char c)
{
    return is_letter(c) || is_digit(c);
}

/* Validate Iranian mobile number */
int is_valid_phone(const char *text, size_t length)
{
    if (length != 11)
        return 0;

    if (text[0] != '0' || text[1] != '9')
        return 0;

    for (size_t i = 0; i < length; i++) {
        if (!is_digit(text[i]))
            return 0;
    }

    return 1;
}

/* Validate IPv4 address */
int is_valid_ipv4(const char *text, size_t length)
{
    size_t start = 0;
    int parts = 0;

    for (size_t i = 0; i <= length; i++) {
        if (i == length || text[i] == '.') {
            size_t part_length = i - start;

            if (part_length < 1 || part_length > 3)
                return 0;

            if (part_length > 1 && text[start] == '0')
                return 0;

            int value = 0;

            for (size_t j = start; j < i; j++) {
                if (!is_digit(text[j]))
                    return 0;

                value = value * 10 + (text[j] - '0');
            }

            if (value > 255)
                return 0;

            parts++;
            start = i + 1;
        }
    }

    return parts == 4;
}

/* Validate one local-part label */
int valid_local_label(const char *text, size_t start, size_t end)
{
    if (start == end)
        return 0;

    if (!is_alphanumeric(text[start]) ||
        !is_alphanumeric(text[end - 1]))
        return 0;

    for (size_t i = start; i < end; i++) {
        char c = text[i];

        if (!is_alphanumeric(c) && c != '-' && c != '_')
            return 0;
    }

    return 1;
}

/* Validate one domain label */
int valid_domain_label(const char *text, size_t start, size_t end)
{
    if (start == end)
        return 0;

    if (!is_alphanumeric(text[start]) ||
        !is_alphanumeric(text[end - 1]))
        return 0;

    for (size_t i = start; i < end; i++) {
        if (!is_alphanumeric(text[i]) && text[i] != '-')
            return 0;
    }

    return 1;
}

/* Validate email address */
int is_valid_email(const char *text, size_t length)
{
    size_t at = length;
    int at_count = 0;

    /* Find exactly one @ */
    for (size_t i = 0; i < length; i++) {
        if (text[i] == '@') {
            at = i;
            at_count++;
        }
    }

    if (at_count != 1 || at == 0 || at == length - 1)
        return 0;

    /* Validate local part */
    size_t start = 0;

    for (size_t i = 0; i <= at; i++) {
        if (i == at || text[i] == '.') {
            if (!valid_local_label(text, start, i))
                return 0;

            start = i + 1;
        }
    }

    /* Find the last dot in the domain */
    size_t last_dot = length;

    for (size_t i = at + 1; i < length; i++) {
        if (text[i] == '.')
            last_dot = i;
    }

    /* Domain must contain at least two labels */
    if (last_dot == length)
        return 0;

    /* Validate domain labels before the TLD */
    start = at + 1;

    for (size_t i = start; i < last_dot; i++) {
        if (text[i] == '.') {
            if (!valid_domain_label(text, start, i))
                return 0;

            start = i + 1;
        }
    }

    if (!valid_domain_label(text, start, last_dot))
        return 0;

    /* Validate TLD: at least two ASCII letters */
    if (length - last_dot - 1 < 2)
        return 0;

    for (size_t i = last_dot + 1; i < length; i++) {
        if (!is_letter(text[i]))
            return 0;
    }

    return 1;
}

/* Identify the candidate type */
const char *match_pattern(const char *text, size_t length)
{
    if (is_valid_email(text, length))
        return "EMAIL";

    if (is_valid_phone(text, length))
        return "PHONE";

    if (is_valid_ipv4(text, length))
        return "IP";

    return NULL;
}
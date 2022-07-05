#include <math.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>

#include "print_funcs.h"

#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];
int buffer_index = 0;

int print_conversion(const char **format_p, va_list *ap, int char_len) {
    (*format_p)++; // Skip % character

    int res = 0;
    switch (**format_p) {
        case 's':
            res = print_str(va_arg(*ap, const char*));
            break;
        case 'c':
            res = print_char(va_arg(*ap, int));
            break;
        case 'd':
        case 'i':
            res = print_long(va_arg(*ap, long));
            break;
        case 'o':
            res = print_unsigned_long(va_arg(*ap, unsigned long), 8);
            break;
        case 'u':
            res = print_unsigned_long(va_arg(*ap, unsigned long), 10);
            break;
        case 'x':
            res = print_unsigned_long(va_arg(*ap, unsigned long), 16);
            break;
        case '%':
            print_char('%');
            res = 1;
            break;
        case 'n':
            print_long(char_len);
            res = 1;
            break;
        default:
            print_char('%');
            print_buffer_if_full();
            print_char(**format_p);
            res = 2;
    }

    (*format_p)++;
    return res;
}

void clear_buffer(void) {
  buffer_index = 0;
  buffer[buffer_index] ='\0';
}

int print_buffer(void) {
    int status = fputs(buffer, stdout);

    // Return -1 if printing fails
    if (status < 0 || status == EOF) {
        return -1;
    }

    // Reset buffer
    buffer_index = 0;
    buffer[buffer_index] = '\0';

    return status;
}

int print_buffer_if_full(void) {
    if (buffer_index + 1 == BUFFER_SIZE) {
        return print_buffer();
    }

    return 0;
}

/*
  Print string to buffer and clear buffer if full
  Returns number of chars printed or a negative number if an error occurs
*/
int print_str(const char * str) {
    int status = 0;
    int i = 0;

    while (*str) {
        print_char(*str);

        status = print_buffer_if_full();
        if (status < 0) {
            return status;
        }
        i++;
        str++;
    }

    return i;
}

/*
  Print char to buffer
  Returns number of chars printed (1)
*/
int print_char(char c) {
    buffer[buffer_index++] = c;
    buffer[buffer_index] = '\0';
    return 1;
}

/*
  Prints long int to buffer and clear buffer to screen if full.
  Returns number of chars printed to screen
*/
int print_long(long n) {
    // Store result here to reverse it
    int digits_length = ceil(log(LONG_MAX) / log(10));
    char str[digits_length + 2];
    int i = 0;
    int status = 0;

    // Add sign
    if (n < 0) {
        str[i++] = '-';
        n = -n;
    }

    while (n) {
        str[i++] = (n % 10) + '0';
        n /= 10;
    }

    int len = i; // Number of chars to return
    i--;

    // Add number to buffer from reverse;
    while (i >= 0) {
        print_char(str[i]);
        i--;

        status = print_buffer_if_full();
        if (status < 0) {
            return status;
        }
    }

    return len;
}

int print_unsigned_long(unsigned long n, int base) {
    // Store result here to reverse it
    int digits_length = ceil(log(ULONG_MAX) / log(base));
    char str[digits_length + 1]; // Add 1 for null character

    int i = 0;
    int status = 0;

    while (n) {
        int d = (n % base);
        char c = (d < 10) ? d + '0' : d - 10 + 'a';
        str[i++] = c;
        n /= base;
    }

    int len = i; // Number of chars to return
    i--;

    // Add number to buffer from reverse;
    while (i >= 0) {
        print_char(str[i]);
        i--;

        status = print_buffer_if_full();
        if (status < 0) {
            return status;
        }
    }

    return len;
}

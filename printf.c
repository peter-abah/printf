#include <stdarg.h>
#include <stdio.h>
#include "printf.h"

#define BUFFER_SIZE 1024

// Uooer bound of number of digits in a long
#define DIGITS_IN_LONG  3 * sizeof(long)

int print_buffer(void);
int print_buffer_if_full(void);
int print_str(const char * str);
int print_char(char c);
int print_decimal(long n);
int print_hex(unsigned long n);
int print_conversion(const char **format_p, va_list *ap, int char_len);

char buffer[BUFFER_SIZE];
int buffer_index = 0;

// Custom implementation of printf
int myprintf(const char *format, ...) {
    va_list ap;
    va_start(ap, format);

    int i = 0;
    int status = 0;
    buffer[i] = '\0';

    while (*format) {
        switch (*format) {
            case '%':;
                int res = print_conversion(&format, &ap, i);
                if (res < 0) {
                    return res;
                }
                i += res;
                break;
            default:
                buffer[buffer_index++] = *format;
                buffer[buffer_index] = '\0';
                i++;
                format++;
        }

        status = print_buffer_if_full();
        if (status < 0) {
            return status;
        }
    }

    va_end(ap);

    status = print_buffer();
    if (status < 0) {
        return status;
    }

    return i;
}

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
            res = print_decimal(va_arg(*ap, long));
            break;
        case 'x':
            res = print_hex(va_arg(*ap, unsigned long));
            break;
        case '%':
            print_char('%');
            res = 1;
            break;
        case 'n':
            print_decimal(char_len);
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
        buffer[buffer_index++] = *str;
        buffer[buffer_index] = '\0';

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
  Prints int, short int and long int to buffer and
  clear buffer to screen if full.
  Returns number of chars printed to screen
*/
int print_decimal(long n) {
    // Store result here to reverse it
    char str[DIGITS_IN_LONG + 2];
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
        buffer[buffer_index++] = str[i];
        buffer[buffer_index] = '\0';
        i--;

        status = print_buffer_if_full();
        if (status < 0) {
            return status;
        }
    }

    return len;
}

int print_unsigned_decimal(unsigned long n) {
    // Store result here to reverse it
    char str[DIGITS_IN_LONG + 2]; // Add 2 for sign and null character
    int i = 0;
    int status = 0;

    while (n) {
        str[i++] = (n % 10) + '0';
        n /= 10;
    }

    int len = i; // Number of chars to return
    i--;
    // Add number to buffer from reverse;
    while (i >= 0) {
        buffer[buffer_index++] = str[i];
        buffer[buffer_index] = '\0';
        i--;

        status = print_buffer_if_full();
        if (status < 0) {
            return status;
        }
    }

    return len;
}

int print_hex(unsigned long n) {
    // Store result here to reverse it
    char str[sizeof(long) * 2]; // One hex digit is 4 bits
    int i = 0;
    int status = 0;

    while (n) {
        int d = (n % 16);
        char c = (d < 10) ? d + '0' : d - 10 + 'a';
        str[i++] = c;
        n /= 16;
    }

    int len = i; // Number of chars to return
    i--;

    // Add number to buffer from reverse;
    while (i >= 0) {
        buffer[buffer_index++] = str[i];
        buffer[buffer_index] = '\0';
        i--;

        status = print_buffer_if_full();
        if (status < 0) {
            return status;
        }
    }

    return len;
}

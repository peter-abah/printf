#include <stdarg.h>
#include <stdio.h>
#include "printf.h"

#define BUFFER_SIZE 1024
#define CONVERT_FNS_SIZE 1

typedef int (*convert_fn)(va_list ap);

// Map conversion specifier with their functions
typedef struct {
    char c;
    convert_fn fn;
} convert_fn_table_t;

int print_buffer(void);
int print_str(va_list ap);
convert_fn get_func(char c);

convert_fn_table_t convert_fn_table[CONVERT_FNS_SIZE] = {
    {'s', print_str}
};

char buffer[BUFFER_SIZE];
int buffer_index = 0;

// Custom implementation of printf
int myprintf(const char *format, ...) {
    va_list ap;
    va_start(ap, format);

    int i = 0;
    int status = 0;
    int in_conversion = 0;

    while (*format) {
        if (in_conversion) {
            convert_fn fn = get_func(*format);
            if (fn != NULL) {
                int res = fn(ap);
                if (res < 0) {
                    return res;
                }
                i += res;
                in_conversion = 0;
            }
            else {
                // TODO: Print with %
                buffer[buffer_index++] = *format;
                buffer[buffer_index] = '\0';
                i++;
            }
        }
        else if (*format == '%') {
            in_conversion = 1;
        }
        else {
            buffer[buffer_index++] = *format;
            buffer[buffer_index] = '\0';
            i++;
        }

        // Flush buffer to screen when full
        if (buffer_index + 1 == BUFFER_SIZE) {
            status = print_buffer();
            if (status < 0) {
                return status;
            }
        }

        format++;
    }

    va_end(ap);

    if (buffer_index > 0) {
        status = print_buffer();
        if (status < 0) {
            return status;
        }
    }

    return i;
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

/*
  Print string to buffer and clear buffer if full
  Returns number of chars printed or a negative number if an error occurs
*/
int print_str(va_list ap) {
    int status = 0;
    int i = 0;
    const char *str = va_arg(ap, const char*);

    while (*str) {
        buffer[buffer_index++] = *str;
        buffer[buffer_index] = '\0';

        if (buffer_index + 1 == BUFFER_SIZE) {
            status = print_buffer();
            if (status < 0) {
                return status;
            }
        }
        i++;
        str++;
    }

    return i;
}

/*
  Returns appropriate function for specifier
*/
convert_fn get_func(char c) {
    for (int i = 0; i < CONVERT_FNS_SIZE; i++) {
        if (convert_fn_table[i].c == c) {
            return convert_fn_table[i].fn;
        }
    }
    return NULL;
}

#include <stdarg.h>
#include <stdio.h>
#include "printf.h"
#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];

int print_buffer(void);

// Custom implementation of printf
int myprintf(const char *format, ...) {
    va_list ap;
    va_start(ap, format);

    int buffer_index = 0;
    int i = 0;
    int status = 0;

    while (*format) {
        buffer[buffer_index++] = *format;
        buffer[buffer_index] = '\0';
        i++;

        // Flush buffer to screen when full
        if (buffer_index + 1 == BUFFER_SIZE) {
            status = print_buffer();
            buffer_index = 0;
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
    buffer[0] = '\0';

    return status;
}

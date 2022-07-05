#include <stdarg.h>
#include "printf.h"
#include "print_funcs.h"

// Custom implementation of printf
int myprintf(const char *format, ...) {
    va_list ap;
    va_start(ap, format);

    int i = 0;
    int status = 0;
    clear_buffer();

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
                print_char(*format);
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


#include <math.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>

#include "print_funcs.h"
#include "process_options.h"

#define BUFFER_SIZE 4

int print_str(const char * str, print_flags flags);
int print_long(long n, print_flags flags);
int print_unsigned_long(unsigned long n, int base, print_flags flags);
int print_hex(unsigned long n, print_flags flags);
int print_octal(unsigned long n, print_flags flags);

char buffer[BUFFER_SIZE];
int buffer_index = 0;

int print_conversion(const char **format_p, va_list *ap, int char_len) {
    (*format_p)++; // Skip % character

    int res = 0;

    // Get flags and store in flags struct
    print_flags flags = {0, 0, 0, 0, 0, 0};
    process_flags(format_p, &flags);

    switch (**format_p) {
        case 's':
            res = print_str(va_arg(*ap, const char*), flags);
            break;
        case 'c':
            res = print_char(va_arg(*ap, int));
            break;
        case 'd':
        case 'i':
            res = print_long(va_arg(*ap, long), flags);
            break;
        case 'o':
            res = print_octal(va_arg(*ap, unsigned long), flags);
            break;
        case 'u':
            res = print_unsigned_long(va_arg(*ap, unsigned long), 10, flags);
            break;
        case 'X':
            flags.capital_letters = 1;
            // No break here is intentional meant to fall through
        case 'x':
            res = print_hex(va_arg(*ap, unsigned long), flags);
            break;
        case '%':
            print_char('%');
            res = 1;
            break;
        case 'n':
            res = print_long(char_len, flags);
            break;
        case '\0':
            res = 0;
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
  Print char to buffer
  Attempts to clear buffer to screen if full before printing cahr
  Returns number of chars printed (1)
*/
int print_char(char c) {
    int status = print_buffer_if_full();
    if (status < 0) {
        return status;
    }

    buffer[buffer_index++] = c;
    buffer[buffer_index] = '\0';
    return 1;
}

/*
  Print string to buffer
  Returns number of chars printed or a negative number if an error occurs
*/
int print_str(const char * str, print_flags flags) {
    int status = 0;
    int i = 0;

    while (*str) {
        status = print_char(*str);
        if (status < 0) {
            return status;
        }

        i++;
        str++;
    }

    return i;
}

/*
  Prints long int to buffer and clear buffer to screen if full.
  Returns number of chars printed to screen
*/
int print_long(long n, print_flags flags) {
    // Store result here to reverse it
    int digits_length = ceil(log(LONG_MAX) / log(10));
    char str[digits_length + 2];
    int i = 0;

    int len = 0; // Count no of chars printes
    int status = 0;

    // Add sign
    if (n < 0) {
        len = print_char('-');

        // Print last digit since converting to positive will overflow
        str[i++] = -(n % 10) + '0';
        n = -(n / 10);
    }
    // Add positive sign if flag is set
    else if (flags.sign) {
        len = print_char('+');
    }
    // Add space if flag is set
    else if (flags.space) {
        len = print_char(' ');
    }

    if (len < 0) {
        return len;
    }

    // Add digits to string in reverse
    while (n) {
        str[i++] = (n % 10) + '0';
        n /= 10;
    }

    len += i; // Add digits to len
    i--;

    // Add number to buffer from reverse;
    while (i >= 0) {
        status= print_char(str[i]);
        if (status < 0) {
            return status;
        }

        i--;
    }

    return len;
}

int print_unsigned_long(unsigned long n, int base, print_flags flags) {
    int digits_length = ceil(log(ULONG_MAX) / log(base));

    // Store result here to reverse it

    char str[digits_length + 1]; // Add 1 for null character

    int i = 0;
    int status = 0;
    char letter = flags.capital_letters ? 'A' : 'a';

    while (n) {
        int d = (n % base);
        char c = (d < 10) ? d + '0' : d - 10 + letter;
        str[i++] = c;
        n /= base;
    }

    int len = i; // Number of chars to return
    i--;

    // Add number to buffer from reverse;
    while (i >= 0) {
        status = print_char(str[i]);
        if (status < 0) {
            return status;
        }

        i--;
    }

    return len;
}

int print_hex(unsigned long n, print_flags flags) {
    int i = 0;

    if (flags.alternate && n > 0) {
        print_flags empty_flags = {0, 0, 0, 0, 0};

        if (flags.capital_letters) {
            i = print_str("0X", empty_flags);
        } else {
            i = print_str("0x", empty_flags);
        }
    }

    if (i < 0) {
        return i;
    }

    int res = print_unsigned_long(n, 16, flags);
    if (res < 0) {
        return res;
    }

    return i + res;
}

int print_octal(unsigned long n, print_flags flags) {
    int i = 0;

    if (flags.alternate) {
        i = print_char('0');
    }

    if (i < 0) {
        return i;
    }

    int res = print_unsigned_long(n, 8, flags);
    if (res < 0) {
        return res;
    }

    return i + res;
}

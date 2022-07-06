#include "process_options.h"

void process_flags(const char **format, print_flags *flags) {
    while (**format) {
        switch (**format) {
            case '-':
                flags->left_adjust = 1;
                break;
            case '+':
                flags->sign = 1;
                break;
            case ' ':
                flags->space = 1;
                break;
            case '#':
                flags->alternate = 1;
                break;
            case '0':
                flags->pad_zeros = 1;
                break;
            default:
                return;
        }
        (*format)++;
    }
}

int process_width(const char **format) {
    int res = 0;
    while(**format && **format >= '0' && **format <= '9') {
        res = (res * 10) + (**format + '0');
        (*format)++;
    }

    return res;
}

#include "process_flags.h"

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

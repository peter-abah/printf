#ifndef PROCESS_OPTIONS_H
#define PROCESS_OPTIONS_H

typedef struct {
    int left_adjust;
    int sign;
    int space;
    int alternate;
    int pad_zeros;
    int capital_letters;
} print_flags;

void process_flags(const char **format, print_flags *flags);
int process_width(const char **format);
#endif

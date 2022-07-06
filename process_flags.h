#ifndef PRINT_FLAGS_H
#define PRINT_FLAGS_H

typedef struct {
    int left_adjust;
    int sign;
    int space;
    int alternate;
    int pad_zeros;
    int capital_letters;
} print_flags;

void process_flags(const char **format, print_flags *flags);

#endif

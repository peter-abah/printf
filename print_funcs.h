#ifndef PRINT_FUNCS_H
#define PRINT_FUNCS_H

void clear_buffer(void);
int print_buffer(void);
int print_buffer_if_full(void);
int print_conversion(const char **format_p, va_list *ap, int char_len);
int print_char(char c);

#endif

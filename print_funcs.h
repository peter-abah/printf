#ifndef PRINT_FUNCS_H
#define PRINT_FUNCS_H

void clear_buffer(void);
int print_buffer(void);
int print_buffer_if_full(void);
int print_str(const char * str);
int print_char(char c);
int print_long(long n);
int print_unsigned_long(unsigned long n, int base);
int print_hex(unsigned long n);
int print_conversion(const char **format_p, va_list *ap, int char_len);

#endif
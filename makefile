main: main.c libmyprintf.a
	gcc main.c -L. -lmyprintf -lm -o main

libmyprintf.a: printf.o print_funcs.o process_options.o
	ar -rcs libmyprintf.a printf.o print_funcs.o process_options.o

printf.o: printf.c printf.h print_funcs.h
	gcc -c printf.c

print_funcs.o: print_funcs.c print_funcs.h process_options.h
	gcc -c print_funcs.c

process_options.o: process_options.c process_options.h
	gcc -c process_options.c

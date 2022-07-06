main: main.o printf.o print_funcs.o process_flags.o
	gcc main.o printf.o print_funcs.o process_flags.o -o main -lm

main.o: main.c printf.h print_funcs.h
	gcc -c main.c

printf.o: printf.c printf.h print_funcs.h
	gcc -c printf.c

print_funcs.o: print_funcs.c print_funcs.h process_flags.h
	gcc -c print_funcs.c

process_flags.o: process_flags.c process_flags.h
	gcc -c process_flags.c

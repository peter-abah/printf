print_funcs.o: print_funcs.c print_funcs.h
	gcc -c print_funcs.c

printf.o: printf.c printf.h print_funcs.h
	gcc -c printf.c

main.o: main.c printf.h print_funcs.h
	gcc -c main.c

main: main.o printf.o print_funcs.o
	gcc main.o printf.o print_funcs.o -o main -lm

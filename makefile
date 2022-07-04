main: main.o printf.o
	gcc main.o printf.o -o main

main.o: main.c printf.h
	gcc -c main.c

printf.o: printf.c printf.h
	gcc -c printf.c

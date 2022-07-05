main: main.o printf.o
	gcc -g main.o printf.o -o main -lm

main.o: main.c printf.h
	gcc -c main.c

printf.o: printf.c printf.h
	gcc -c printf.c

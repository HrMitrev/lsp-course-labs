.DEFAULT_GOAL := prog
.PHONY: clean

CC = gcc
RM = rm

prog : main.o
	@echo "final linking..."
	@${CC} main.o -o prog 

main.o : main.c
	@echo "compiling main.c"
	@${CC} main.c -c -o main.o

clean :
	@echo "cleaning"
	@${RM} -rf main.o calc_sum.o prog

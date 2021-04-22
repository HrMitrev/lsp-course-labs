.DEFAULT_GOAL := prog
.PHONY: clean

CC = gcc
RM = rm

prog : main.o calc_sum.o
	@echo "final linking..."
	@${CC} calc_sum.o main.o -o prog -lpthread

main.o : main.c
	@echo "compiling main.c"
	@${CC} main.c -c -o main.o -lpthread

calc_sum.o : calc_sum.c
	@echo "compiling calc_sum.c"
	@${CC} calc_sum.c -c -o calc_sum.o

clean :
	@echo "cleaning"
	@${RM} -rf main.o calc_sum.o prog

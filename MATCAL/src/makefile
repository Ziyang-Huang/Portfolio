DIR_SRC = ./src
SRC = $(wildcard ${DIR_SRC}/*.c) 
OBJ = main.o interact.o matrix.o fileIO.o fraction.o\
vector.o math.o stack.o cmd.o

matcal: $(OBJ)
	gcc -g -o $@ $^

$(OBJ): %.o: %.c
	gcc -g -c $<

clean:
	rm matcal $(OBJ)

cnt:
	wc main.c interact.c matrix.c fileIO.c fraction.c vector.c math.c stack.c cmd.c\
		Define.h interact.h matrix.h fraction.h vector.h math.h stack.h cmd.h


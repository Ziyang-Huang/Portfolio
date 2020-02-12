myshell: main.o getcmd.o cmd.o
	gcc -g -o myshell main.o getcmd.o cmd.o -lcurses
main.o: main.c
	gcc -g -c main.c
getcmd.o: getcmd.c
	gcc -g -c getcmd.c
cmd.o: cmd.c
	gcc -g -c cmd.c -lcurses
clean:
	rm main.o getcmd.o cmd.o myshell

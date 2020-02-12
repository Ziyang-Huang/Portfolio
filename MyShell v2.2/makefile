all: myshell wc cp cat man cmp

myshell: main.o getcmd.o common.o
	gcc -g -o myshell main.o getcmd.o common.o
main.o: srcfile/main.c srcfile/common.h
	gcc -g -c srcfile/main.c srcfile/common.h
getcmd.o: srcfile/getcmd.c srcfile/common.h
	gcc -g -c srcfile/getcmd.c srcfile/common.h
common.o: srcfile/common.c srcfile/common.h
	gcc -g -c srcfile/common.c srcfile/common.h

wc: wc.o common.o
	gcc -g -o wc wc.o common.o
wc.o: srcfile/wc.c srcfile/common.h
	gcc -g -c srcfile/wc.c srcfile/common.h

cp: cp.o common.o
	gcc -g -o cp cp.o common.o
cp.o: srcfile/cp.c srcfile/common.h
	gcc -g -c srcfile/cp.c srcfile/common.h

cat: cat.o common.o
	gcc -g -o cat cat.o common.o
cat.o: srcfile/cat.c srcfile/common.h
	gcc -g -c srcfile/cat.c srcfile/common.h

man: man.o common.o
	gcc -g -o man man.o common.o -lcurses
man.o: srcfile/man.c srcfile/common.h
	gcc -g -c srcfile/man.c srcfile/common.h

cmp: cmp.o common.o
	gcc -g -o cmp cmp.o common.o
cmp.o: srcfile/cmp.c srcfile/common.h
	gcc -g -c srcfile/cmp.c srcfile/common.h

clean:
	rm main.o getcmd.o cp.o man.o cmp.o cat.o wc.o common.o myshell wc cp cat man cmp

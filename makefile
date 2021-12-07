all: control.o writing.o
	gcc -o control control.o
	gcc -o writing writing.o

control.o: control.c
	gcc -c control.c

writing.o: writing.c
	gcc -c writing.c

clean:
	rm *.o
	rm control
	rm writing

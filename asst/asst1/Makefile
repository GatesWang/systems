memgrind.o: mymalloc.o memgrind.h
	gcc -o memgrind -c memgrind.c memgrind.h mymalloc.o

mymalloc.o:
	gcc -c mymalloc.c

clean:
	rm mymalloc.o

#include <stdio.h>
#include "malloc.c"

int main(int argc, char*argv[]){
	malloc(5000);
	malloc(3);
	print_memory(0,20);
	shift(0,2);
	print_memory(0,20);

}

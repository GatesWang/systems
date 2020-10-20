
#include <stdio.h>

#define malloc(x) mymalloc(x,__FILE__,__LINE__)
#define free(x) myfree(x,__FILE__,__LINE__)

static char myblock[4096];
void *pointers[4096];
int insertion = 0;

void myfree(void *pointer, char *file, int line){

}

/*
	prints out range in myblock, inclusive
*/
void print_memory(int start, int end){
	for(int i=start; i<=end; i++){
		printf("%d:%c ",i,myblock[i]);
	}
	printf("\n");
}

/*
	input: the range of a block, [start, end]
	this attempts to fill in the block by shifting everything
	to the right of the block over
*/
void shift(int start, int end){
	int write = start;
	for(int read=end+1; read<20; read++){
		myblock[write] = myblock[read];
		//print_memory(start,end);
		write++;
	}
}

void * mymalloc(size_t size, char *file, int line){
	char *start = &myblock[0];
	if(insertion + size > 4096){
		printf("error\n");
	}
	else{
		for(int i=0; i<size; i++){
			myblock[insertion] = 'a';
			insertion++;
		}
	}
	//free();
	printf("\n");
	return start;
}


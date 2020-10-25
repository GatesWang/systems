#include "mymalloc.c"

/*
	This will allocate or free a pointer.
	It is assumed that everything in [0, pointer_index] is a valid poitner to free.
	It is assumed that everything in [pointer_index+1, length-1] is valid storage for a new pointer.
*/
void allocate_or_free(void* pointers[], int length, int* pointer_index, int free, int create, int size){
	int num = rand() % (free+create);
	//free
	if(num < free){
		if((*pointer_index) >= 0){
			free(pointers[(*pointer_index)--]);
		}
	}
	//allocate
	else{
		if((*pointer_index) < length){
			pointers[++(*pointer_index)] = malloc(size);
		}
	}
}

/*
	This will free all of the pointers stored in the array.
	It is assumed that everything in [0, pointer_index] is a valid pointer to free.
*/
void free_pointers(void* pointers[], int length, int* pointer_index){
	for(; (*pointer_index)>=0; (*pointer_index)--){
		free(pointers[*(pointer_index)]);
	}
}

/*
	These are the tests provided in the assignment.
*/
void basic_tests(){
	printf("1: should be not enough memory\n");
	malloc(5000);

	printf("2: should be not enough memory\n");
	void *pointer = malloc(4096);
	malloc(1);
	free(pointer);

	printf("3: should be not allocated by malloc\n");
	char *p = (char*) malloc(200);
	free(p+10);

	printf("4: should be not allocated by malloc\n");
	int *y;
	free(y);

	printf("5: should be not allocated by malloc\n");
	int x;
	free((int*)x);

	printf("6: should be not allocated by malloc\n");
	p =(char *) malloc(100);
	free(p);
	free(p);

	printf("7: should be no error\n");
	p =(char *) malloc(100);
	free(p);
	p =(char *) malloc(100);
	free(p);
}

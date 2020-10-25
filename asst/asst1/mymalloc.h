#include <stdio.h>
#define malloc(x) mymalloc(x,__FILE__,__LINE__)
#define free(x) myfree(x,__FILE__,__LINE__)

typedef struct Metadata{
	void *ptr;
	int start;
	int end;
} Metadata;


Metadata createData(void *ptr, int start, int end){
	Metadata data;
	data.ptr = ptr;
	data.start = start;
	data.end = end;
	return data;
}

static char myblock[4096];
static Metadata data[4096];
int memory_insertion = 0;
int data_insertion = 0;

/*
	Given a pointer we determine whether or not the address is from our memory block.
*/
int is_in_range(void *ptr){
	void *start = &myblock[0];
	void *end = &myblock[4096];
	if(ptr>=start && ptr<=end){
		return 1;
	}
	else{
		return 0;
	}
}

/*
	Given a range [start, end] we erase that range in our memory block by shifting the rightmost elements
	to the left. It will change the insertion point for the memory block.
*/
void remove_memory_block(int start, int end){
	int write = start;
	for(int read=end+1; read<4096; read++){
		myblock[write++] = myblock[read];
	}
	int length = end - start + 1;
	memory_insertion -= length;
}

/*
	Given an index this removes the metadata stored at index by shifting the elements on the right to the left.
	It will change the insertion point for metadata.
*/
void remove_data_at_index(int index){
	int write = index;
	for(int read=index+1; read<4096; read++){
		data[write++] = data[read];
	}
	data_insertion--;
}

/*
	This prints the state of our memory block. It includes the amount of memory used, and the number of
	pointers stored.
*/
void print_info(char* string){
	printf("%s memory: %d, pointers: %d\n", string, memory_insertion, data_insertion);
}

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

int is_in_range(void *ptr);
void print_info(char* string);

void remove_memory_block(int start, int end);
void remove_data_at_index(int index);
void insert_data(Metadata data);

void myfree(void *pointer, char *file, int line){
	int valid = is_in_range(pointer);
	if(valid == 0){
		printf("not in range\n");
		return;
	}
	for(int i=0; i<4096; i++){
		if(data[i].ptr == pointer){
			Metadata metadata = data[i];
			remove_memory_block(metadata.start, metadata.end);
			remove_data_at_index(i);
			return;
		}
	}
	printf("not allocated by malloc\n");
}

void* mymalloc(size_t size, char *file, int line){
	if(memory_insertion + size > 4096){
		printf("not enough memory\n");
		return NULL;
	}
	else{
		int start = memory_insertion;
		void *pointer = &myblock[memory_insertion];
		for(int i=0; i<size; i++){
			myblock[memory_insertion++] = 'a';
		}
		int end = memory_insertion-1;
		Metadata metadata = createData(pointer, start, end);
		data[data_insertion++] = metadata;
		return pointer;
	}
}

/*
	input: a pointer
	this determines whether the pointer given is a valid address
	in myblock
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
	input: the range of a block, [start, end]
	this attempts to fill in the block by shifting everything
	to the right of the block over
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
	This removes the metadata stored at index, while shifting everything to fill in the gap created.
*/
void remove_data_at_index(int index){
	int write = index;
	for(int read=index+1; read<4096; read++){
		data[write++] = data[read];
	}
	data_insertion--;
}

void print_info(char* string){
	printf("%s memory: %d, pointers: %d\n", string, memory_insertion, data_insertion);
}

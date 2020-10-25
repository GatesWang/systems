#include "mymalloc.h"

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

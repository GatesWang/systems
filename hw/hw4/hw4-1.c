#include <stdio.h>

void* print_ints(void *ptr){
	for(int i=1; i<=10; i++){
		printf("%d ", i);
		printf("\n");
	}
}


int main(int argc, char* argv[]){
	int number = 1;
	print_ints(&number);
	return 0;
}

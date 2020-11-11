#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void* print_ints(void *ptr){
	int *number = (int*) ptr;
	for(int i=1; i<=10; i++){
		printf("%d: %d \n", *number, i);
	}
}

int main(int argc, char* argv[]){
	pthread_t tids[5];

	for(int i=0; i<5; i++){
		pthread_create(&tids[i], NULL, print_ints, &i);
	}
	for(int i=0; i<5; i++){
		pthread_join(tids[i], NULL);
	}
	return 0;
}

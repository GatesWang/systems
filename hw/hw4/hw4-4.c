
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_t tids[5];
pthread_mutex_t lock;

void* print_ints(void *ptr){
	pthread_mutex_t *mutex = (pthread_mutex_t*) ptr;
	pthread_mutex_lock(mutex);
	for(int i=1; i<=10; i++){
		printf("%lu, %d \n", pthread_self(), i);
	}
	pthread_mutex_unlock(mutex);
}

int main(int argc, char* argv[]){
	for(int i=0; i<5; i++){
		pthread_create(&tids[i], NULL, print_ints, &lock);
	}
	for(int i=0; i<5; i++){
		pthread_join(tids[i], NULL);
	}
	return 0;
}

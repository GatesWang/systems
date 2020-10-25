#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "malloc.c"

suseconds_t workload_a();
suseconds_t workload_b();
suseconds_t workload_c();
suseconds_t workload_d();
suseconds_t workload_e();

void allocate_or_free(void* pointers[], int length, int* pointer_index, int free, int create, int size);
void free_pointers(void* pointers[], int length, int* pointer_index);
void basic_tests();

int main(int argc, char*argv[]){
	double time_a = 0;
	double time_b = 0;
	double time_c = 0;
	double time_d = 0;
	double time_e = 0;

	for(int workload=0; workload<50; workload++){
		time_a += workload_a();
		time_b += workload_b();
		time_c += workload_c();
		time_d += workload_d();

		time_e += workload_e();
		printf("\n");
	}

	time_a = time_a/50;
	time_b = time_b/50;
	time_c = time_c/50;
	time_d = time_d/50;
	time_e = time_e/50;
	char output[] ="workload a: %f\nworkload b: %f\nworkload c: %f\nworkload d: %f\nworkload e: %f\n";
	printf(output, time_a,time_b,time_c,time_d,time_e);
}


/*
	returns how long the workload took to execute
*/
suseconds_t workload_a(){
	struct timeval difference;
	struct timeval start_time;
	struct timeval end_time;
	gettimeofday(&start_time, NULL);

	for(int i=0; i<120; i++){
		char *ptr = malloc(1);
		free(ptr);
	}

	print_info("workload a: ");
	gettimeofday(&end_time, NULL);
	timersub(&end_time, &start_time, &difference);
	suseconds_t diff  = difference.tv_usec;
	return diff;
}


/*
	returns how long the workload took to execute
*/
suseconds_t workload_b(){
	struct timeval difference;
	struct timeval start_time;
	struct timeval end_time;
	gettimeofday(&start_time, NULL);

	void* pointers[120];
	for(int i=0; i<120; i++){
		pointers[i] = malloc(1);
	}
	for(int i=0; i<120; i++){
		free(pointers[i]);
	}

	print_info("workload b: ");
	gettimeofday(&end_time, NULL);
	timersub(&end_time, &start_time, &difference);
	suseconds_t diff  = difference.tv_usec;
	return diff;
}

/*
	returns how long the workload took to execute
*/
suseconds_t workload_c(){
	struct timeval difference;
	struct timeval start_time;
	struct timeval end_time;
	gettimeofday(&start_time, NULL);

	int length = 120;
	void* pointers[length];
	int pointer_index = -1;
	int free = 1;
	int create = 1;
	int size = 1;

	for(int i=0; i<240; i++){
		allocate_or_free(pointers, length, &pointer_index, free, create, size);
	}
	print_info("workload c: ");
	free_pointers(pointers, length, &pointer_index);

	gettimeofday(&end_time, NULL);
	timersub(&end_time, &start_time, &difference);
	suseconds_t diff  = difference.tv_usec;
	return diff;
}


/*
	returns how long the workload took to execute
*/
suseconds_t workload_d(){
	struct timeval difference;
	struct timeval start_time;
	struct timeval end_time;
	gettimeofday(&start_time, NULL);

	int length = 120;
	void* pointers[length];
	int pointer_index = -1;
	int free = 7;
	int create = 12;
	int size = 1;

	for(int i=0; i<240; i++){
		allocate_or_free(pointers, length, &pointer_index, free, create, size);
	}
	print_info("workload d: ");
	free_pointers(pointers, length, &pointer_index);


	gettimeofday(&end_time, NULL);
	timersub(&end_time, &start_time, &difference);
	suseconds_t diff  = difference.tv_usec;
	return diff;
}

/*
	returns how long the workload took to execute
*/
suseconds_t workload_e(){
	struct timeval difference;
	struct timeval start_time;
	struct timeval end_time;
	gettimeofday(&start_time, NULL);

	int length = 120;
	void* pointers[length];
	int pointer_index = -1;
	int free = 12;
	int create = 7;
	int size = 300;

	for(int i=0; i<240; i++){
		allocate_or_free(pointers, length, &pointer_index, free, create, size);
	}
	print_info("workload e: ");
	free_pointers(pointers, length, &pointer_index);


	gettimeofday(&end_time, NULL);
	timersub(&end_time, &start_time, &difference);
	suseconds_t diff  = difference.tv_usec;
	return diff;
}

/*
	allocates or frees pointers
*/
void allocate_or_free(void* pointers[], int length, int* pointer_index, int free, int create, int size){
	int num = rand() % (free+create);
	if(num < free){//remove
		if((*pointer_index) >= 0){
			free(pointers[(*pointer_index)--]);
		}
	}
	else{//add
		if((*pointer_index) < length){
			pointers[++(*pointer_index)] = malloc(size);
		}
	}
}

/*
	free all of the pointers stored in the array
*/
void free_pointers(void* pointers[], int length, int* pointer_index){
	for(; (*pointer_index)>=0; (*pointer_index)--){
		free(pointers[*(pointer_index)]);
	}
}


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

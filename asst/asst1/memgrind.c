#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.c"
#include "memgrind.h"

suseconds_t workload_a();
suseconds_t workload_b();
suseconds_t workload_c();
suseconds_t workload_d();
suseconds_t workload_e();

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

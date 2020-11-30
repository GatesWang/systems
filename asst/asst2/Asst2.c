#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include "Asst2.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

FileStruct *start = NULL;
FileStruct *currentFile = NULL;
pthread_mutex_t fileLock;

int main(int argc, char *argv[]){
	processDirectory(argv[1]);
	iterateFiles();
	return 0;
}

/*
	perform analysis after multithreading is done.
*/
void iterateFiles(){
	FileStruct *current = start;
	current = start;
	//get total token counts
	while(current != NULL){
		current->tokenCount = getTokenCount(current->tokens);
		calculateFrequencies(current->tokens, current->tokenCount);
		current = current->nextFile;
	}
	sortFiles();
	compareFiles();
	//printFiles();
}

/*
	get the total number of tokens in the file
*/
int getTokenCount(Tokens *tokens){
	int count = 0;
	Tokens *current = tokens;
	Tokens *prev;

	while(current != NULL){
		count += current->count;
		prev = current;
		current = current->next;
	}
	return count;
}

/*
	calculate frequencies for each token
*/
void calculateFrequencies(Tokens *tokens, int total){
	Tokens *current = tokens;
	while(current != NULL){
		current->frequency = (float)current->count/total;
		current = current->next;
	}
}

/*
	print the tokens for each file
*/
void printFiles(){
	int count = 0;
	FileStruct *current = start;
	current = start;
	while(current != NULL){
		printTokens(current->tokens);
		printf("\nnumber tokens: %d", current->tokenCount);
		current = current->nextFile;
		count++;
		printf("\n\n");
	}

	if(count==1){
		printf("Warning there is only one file being analyzed\n");
	}
}

/*
	print tokens for a file
*/
void printTokens(Tokens *tokens){
	Tokens *current = tokens;
	if(tokens==NULL){
		printf("null tokens\n");
	}
	Tokens *prev;
	while(current != NULL){
		printf("%s: %f, ", current->text, current->frequency);
		prev = current;
		current = current->next;
	}
}

/*
	sort files by token count
*/
void sortFiles(){
	int a;
	FileStruct *temp1;
	FileStruct *temp2;

    for(temp1=start;temp1!=NULL;temp1=temp1->nextFile){
    	for(temp2=temp1->nextFile;temp2!=NULL;temp2=temp2->nextFile){
        	if(temp2->tokenCount < temp1->tokenCount){
            	a = temp1->tokenCount;
                temp1->tokenCount = temp2->tokenCount;
                temp2->tokenCount = a;
          	}
       	}
   	}
}

/*
	perform the anaysis
*/
void compareFiles(){
	int a;
	FileStruct *temp1;
	FileStruct *temp2;

   for(temp1=start;temp1!=NULL;temp1=temp1->nextFile){
    	for(temp2=temp1->nextFile;temp2!=NULL;temp2=temp2->nextFile){
			Tokens *f1 = temp1->tokens;
			Tokens *f2 = temp2->tokens;
			float jsd = calculateKLD(f1, f2) + calculateKLD(f2, f1);
			if(jsd <= .1){
				printf(RED "%f, %s and %s\n" RESET, jsd, temp1->path, temp2->path);
			}
			else if (jsd <= .15){
				printf(YEL "%f, %s and %s\n" RESET, jsd, temp1->path, temp2->path);
			}
			else if (jsd <= .2){
				printf(GRN "%f, %s and %s\n" RESET, jsd, temp1->path, temp2->path);
			}
			else if (jsd <= .25){
				printf(CYN "%f, %s and %s\n" RESET, jsd, temp1->path, temp2->path);
			}
			else if (jsd <= .3){
				printf(BLU "%f, %s and %s\n" RESET, jsd, temp1->path, temp2->path);
			}
			else{
				printf(WHT "%f, %s and %s\n" RESET, jsd, temp1->path, temp2->path);
			}
		}
   	}
}

/*
	calculate the kld of two files
*/
float calculateKLD(Tokens *t1, Tokens *t2){
	float answer = 0;
	for(Tokens *t=t1;t!=NULL;t=t->next){
		char *word = t->text;
		float f1 = get(t1,word);
		float f2 = get(t2,word);
		float mean = (f1+f2)/2;
		answer += (f1) * log((f1/mean));
	}
	return answer;
}

/*
	get the frequncy of a word in a file
*/
float get(Tokens *t, char *word){
	for(;t!=NULL;t=t->next){
		if(strcmp(t->text, word)==0){
			return t->frequency;
		}
	}
	return 0;
}

/*
	start a new thread to process a file
*/
void * processFile(void* fileStruct){
	FileStruct * file = (FileStruct*) fileStruct;
	char * filePath = file->path;
	//printf("new thread processing file %s\n", filePath);
	FILE * fptr = fopen(filePath, "r");
  	char *input= readFile(fptr);
	Tokens *tokens = getTokens(input);
	file->tokens = tokens;

	//lock before writing and unlock when done
	pthread_mutex_lock(&fileLock);
	if(!start){
		start = file;
		currentFile = file;
	}
	else{
		currentFile->nextFile = file;
		currentFile = file;
	}
	pthread_mutex_unlock(&fileLock);
	fclose(fptr);
	return NULL;
}

/*
	read a file into a string
*/
char *readFile(FILE * f) {
	int c;
	char * input = malloc(9999);
	size_t n = 0;
	while ((c=fgetc(f)) != EOF) {
		input[n++] = (char) c;
	}
	input[n] = '\0';
	return input;
}

/*
	insert a token into our data structure
*/
Tokens *insertToken(Tokens *tokens, char *substr){
	if(tokens==NULL){
		Tokens *t = (Tokens*) malloc(sizeof(Tokens));
		t->text = strdup(substr);
		t->count = 1;
		return t;
	}

	//not NULL
	Tokens *current = tokens;
	Tokens *prev = NULL;
	while(current != NULL){
		if(strcmp(current->text, substr)==0){
			current->count += 1;
			return tokens;
		}
		prev = current;
		current = current->next;
	}
	//need new node
	if(current == NULL){
		Tokens *newToken = (Tokens*) malloc(sizeof(Tokens));
		newToken->text = substr;
		newToken->count = 1;
		prev->next = newToken;
	}
	return tokens;
}

/*
	break input into tokens and store it in our data structure
*/
Tokens *getTokens(char * input) {
	Tokens *tokens = NULL;
	int start;
	int end;
	char prev = ' ';
	char current = ' ';
	for(int i=0; i<strlen(input); i++) {
		prev = current;
		current = input[i];
		//end of a token
		if(!isspace(prev) && isspace(current)){
			end = i;
			int n = end-start+1;
			if(n>0){
				char * substr = substring(input, start, end-start+1);
				//insert token
				tokens = insertToken(tokens, substr);
			}
		}
		//start of a token
		if(isspace(prev) && !isspace(current)){
			start = i;
		}
	}
	return tokens;
}


/*
	Given the path of a file, create a FileStruct
*/
FileStruct * createFileStruct(char *path){
	FileStruct *file = malloc(sizeof(FileStruct));
	file->path = strdup(path);
	return file;
}

/*
	Given the path of the parent directory and a name, return the full path
*/
char * getFullPath(char * parent, char * name){
	int newSize = strlen(parent) + strlen(name);
	char * fullPath = malloc(sizeof(char) * (newSize + 1));
	if( strlen(parent) > 0){
		fullPath = malloc(sizeof(char) * (newSize + 2)); // need to allocate for '/'
		sprintf(fullPath, "%s/", parent);
	}
	strcat(fullPath, name);
}

char* substring(const char *source, int start, int n){
	char *destination = malloc(sizeof(char) * (n+1));
	char *current = destination;
	while(n > 0){
        *current = *(source + start);
        current++;
        source++;
        n--;
    }
   *current = '\0';
   return destination;
}

/*
	given a directory start a new thread to process it
*/
void * processDirectory(void* input){
	char * directory = (char*) input;
	DIR* dirp = opendir(directory);
	//printf("new thread processing directory: %s\n", directory);
    if(!dirp){
		if(ENOENT == errno){
			printf("directory doesnt exist\n");
		}
		else{
			printf("something else went wrong\n");
		}
	}

	struct dirent * dirent;
	while((dirent = readdir(dirp)) != NULL){
        if(dirent->d_type == DT_DIR){
			if(strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0){
				char * fullPath = getFullPath(directory, dirent->d_name);
				//make recursive call on subdirectories in its own thread
				pthread_t id;
				pthread_create(&id, NULL, processDirectory, fullPath);
				pthread_join(id, NULL);
			}
	  	}
		else{
			char * fullPath = getFullPath(directory, dirent->d_name);
			FileStruct *newFileStruct = createFileStruct(fullPath);
			//process the file in its own thread
			pthread_t id;
			pthread_create(&id, NULL, processFile, newFileStruct);
			pthread_join(id, NULL);
        }
	}
	closedir(dirp);
	return NULL;
}

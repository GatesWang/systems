
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define BACKLOG 5
#define DEBUG 0
#define KNOCK "Knock, knock."
#define WHO "Who's there?"
#define SETUP "set up."
#define PUNCH "punchline!"

char * message_type = NULL;
char * message = NULL;
char * message_length = NULL;

char* substring(const char *source, int start, int n){
	char *destination = (char*) malloc(sizeof(char) * (n+1));
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

void check_error(int error){
	if(error<0){
		printf("There was an error\n");
		exit(1);
	}
}
/*
	Read until a '|' is encountered. This will give you the message type.

	If the type is ERR, read until another '|' is encountered. This will give you the ERR message.
	Return the ERR message.
	If the type is REG, read until another '|' is encountered. This will give you the length.

	Read length bits. Then return this messsage.
*/
char* readInput(int newsockfd){
	int buffer_length = 7, result_length = 10;
	int read_so_far = 0;
	int delim1 = -1, delim2 = -1;
	char * result = (char*) malloc(sizeof(char) * (result_length+1));
	result[0] = '\0';
	char * buffer;

	while(delim1<0 || delim2<0){
		buffer = (char*) malloc(sizeof(char) * (buffer_length+1));
		buffer[0] = '\0';
		read(newsockfd, buffer, buffer_length);
		buffer[buffer_length] = '\0';
		//iterate buffer
		for(int i=0; i<buffer_length; i++){
			char c = buffer[i];
			if(c=='|' && delim1<0){
				delim1 = read_so_far;
			}
			else if(c=='|' && delim2<0){
				delim2 = read_so_far;
			}
			read_so_far++;
		}
		//reallocate space if needed
		if(read_so_far >= result_length){
			result_length *= 2;
			result = realloc(result, result_length);
		}
		strcat(result, buffer);
		if(DEBUG){
			printf("\ndebug 1\n");
			printf("read_so_far: %d\n", read_so_far);
			printf("result_length: %d\n", result_length);

			printf("result: %s\n", result);
			printf("buffer: %s\n", buffer);
		}
	}//end while loop

	//get message_type
	if(message_type == NULL){
		message_type = substring(result, 0, delim1);
		if(strcmp(message_type, "ERR")==0){
			message = substring(result, delim1+1, delim2-delim1-1);
			return result;
		}
	}

	//get message_length
	message_length = substring(result, delim1+1, delim2-delim1-1);

	//get message
	buffer_length = atoi(message_length)+1; //add one for '|' at the end
	buffer = (char*) malloc(sizeof(char) * (buffer_length+1));
	buffer[0] = '\0';
	read(newsockfd, buffer, buffer_length);
	buffer[buffer_length] = '\0';
	strcat(result, buffer);
	message = substring(result, delim2+1, buffer_length);

	if(DEBUG){
		printf("\ndebug 2 \n");

		printf("result: %s\n", result);
		printf("length of result: %ld\n", strlen(result) );

		printf("given length: %s, acutal length: %ld\n", message_length, strlen(message));
		printf("message: %s\n", message);
	}
	return result;
}
/*
	converts a message to Kjj format
*/
char * convertToKjj(char * string){
	//get the length of string as a string
	int length_of_length = snprintf(NULL, 0, "%ld", strlen(string));
	char * length = (char*) malloc(sizeof(char) * (length_of_length+1) );
	sprintf(length, "%ld", strlen(string));

	char * kjj = malloc(sizeof(char) * (7 + strlen(string) + length_of_length));
	sprintf(kjj,"REG|%s|%s|", length, string);
	return kjj;
}

/*
	converts an error message to Kjj format
*/
char * convertToKjjError(char * string){
	char * kjj = malloc(sizeof(char) * (3 + strlen(string)) );
	sprintf(kjj,"ERR|%s|", string);
	return kjj;
}

void send_error(int sockfd, char * error_message){
	write(sockfd, error_message, 4);
	close(sockfd);
	exit(1);
}

int main(int argc, char*argv[]){
	int sockfd, newsockfd, error;
	struct addrinfo *addr_list;
	struct addrinfo *addr;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	error = getaddrinfo(NULL, argv[1], &hints, &addr_list);

	for(addr=addr_list; addr!=NULL; addr=addr->ai_next){
		sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
		if(sockfd>0){
			break;
		}
	}
	check_error(sockfd);
	error = bind(sockfd, addr->ai_addr, addr->ai_addrlen);
	check_error(error);
	error = listen(sockfd, BACKLOG);
	check_error(error);
	newsockfd = accept(sockfd, NULL, NULL);
	check_error(newsockfd);

	char * knock = convertToKjj(KNOCK);
//	write(newsockfd, knock, strlen(knock));

	char* result = readInput(newsockfd);
	int length = strlen(message);
	printf("message: %s\n", message);

	//send ERR message if the REG message received is improper
	if(strcmp(message_type, "REG") == 0){
		int i;
		for(i=0; i<length-1; i++){
			if(message[i] == '|'){// '|' appears early
				send_error(newsockfd, "M1LN");
			}
		}
		if(message[length-1] != '|'){
			send_error(newsockfd, "M1LN");// '|' should be here
		}
		if(strcmp(message, WHO)!=0){
			send_error(newsockfd, "M1CT");
		}
	}
	else if(strcmp(message_type, "ERR") == 0){//we recieved an error, so exit
		char * converted_error = convertToKjjError(message);
		if(strlen(message)!=4){
			printf("error code is invalid");
		}
		else if(strcmp(converted_error, result) !=0){
 			printf("error code format is invalid: %s, should be: %s\n", result, converted_error);
		}
		else{
 			printf("recieved error: %s\n", message);
			exit(1);
		}
	}
	else{
		//send error because the message recieved was invalid, then exit
		send_error(newsockfd, "M1FT");
		exit(1);
	}

	return 0;
}

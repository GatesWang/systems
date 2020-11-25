#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[]) {
	DIR * dirp = opendir(".");
	struct dirent *dirent;
	while((dirent = readdir(dirp)) != NULL){
		if(dirent->d_type == DT_DIR){
	    }
        else{
			//printf("%s\n",dirent->d_name);
			char name[256];
			strcpy(name, dirent->d_name);
			unsigned char type = dirent->d_type;
			FILE *f  = fopen(name,"r");
    		int num = fileno(f);
			int size = lseek(num, 0, SEEK_END);
	    	printf("size of %s: %d\n", dirent->d_name, size);
		}
    }
    closedir(dirp);
}


#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	DIR * dirp = opendir(".");
	struct dirent *dirent;
	while((dirent = readdir(dirp)) != NULL){
	    if(dirent->d_type == DT_DIR){
	        printf("./%s\n", dirent->d_name);
	    }
        else{
            printf("%s\n", dirent->d_name);
        }
    }
    closedir(dirp);
}


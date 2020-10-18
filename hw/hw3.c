#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	DIR * dirp = opendir(".");
	struct dirent *dirent;
	/*
	while((dirent = readdir(dirp)) != NULL){
		printf("%s\n", dirent->d_name);
    }*/
	dirp = opendir(".");

	while((dirent = readdir(dirp)) != NULL){
		if(dirent->d_type == DT_DIR){
			printf("./%s\n", dirent->d_name);
		}
    }

    closedir(dirp);

}


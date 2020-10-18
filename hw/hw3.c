#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	DIR * dirp = opendir(".");
	struct dirent *dirent;

	while((dirent = readdir(dirp)) != NULL){
		printf("%s\n", dirent->d_name);
    }
	dirp = opendir(".");
    closedir(dirp);

}


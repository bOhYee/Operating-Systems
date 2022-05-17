#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <fcntl.h>
#define KNOWN_DIR 2+1
#define MAX_CHAR 250+1

const char curr_dir[KNOWN_DIR] = ".";
const char prv_dir[KNOWN_DIR] = "..";

typedef enum{
    EXEC, SRC, DEST
} arguments;

int checkIfDirectory(char *path);
int createDirectory(char *path);
int copyFile(char *path, char *path_copy);
int copy(char *path, char *path_copy);
int recursiveCopy(char *path, char *path_copy, char *file, int level);

int main(int argc, char **argv) {

    int retValue;

    // Check if there are the right amount of arguments
    if(argc < 3){
        printf("Inserire il path (ass. o rel.) di cui visualizzare il contenuto.\n");
        return -1;
    }

    // Check directory to copy
    if(checkIfDirectory(argv[SRC])){
        printf("The first path is not valid. Check if it's pointing to an actual directory or if it's spelled correctly\n");
        return -1;
    }

    // Check directory to write
    if(checkIfDirectory(argv[DEST])){
        printf("The second path is not valid. Check if it's pointing to an actual directory or if it's spelled correctly\n");
        return -1;
    }

    retValue = copy(argv[SRC], argv[DEST]);
    if(retValue == 0)
        printf("Copy terminated!");

    return retValue;
}

int copy(char *path, char *path_copy){

    // Creating the root of the landing directory
    strcat(path_copy, "/");
    strcat(path_copy, basename(path));
    strcat(path_copy, "_cpy");
    createDirectory(path_copy);

    // Starting the copy
    return recursiveCopy(path, path_copy, "", 0);
}

int recursiveCopy(char *path, char *path_copy, char *file, int level){

    char pathname_A[MAX_CHAR] = "";
    char pathname_B[MAX_CHAR] = "";

    struct stat buf;
    struct dirent *dp;
    DIR *dir;

    strcpy(pathname_A, path);
    strcat(pathname_A, "/");
    strcat(pathname_A, file);

    strcpy(pathname_B, path_copy);
    strcat(pathname_B, "/");
    strcat(pathname_B, file);

    stat(pathname_A, &buf);
    if(S_ISDIR(buf.st_mode)){
        // I need to create the new folder
        if(level != 0){
            // If creating the directory results in an error, the program exits with error code -2
            if(createDirectory(pathname_B) == -1){
                printf("Failed to create a directory in position: %s\n", pathname_B);
                return -2;
            }
        }

        dir = opendir(pathname_A);
        if(dir == NULL) {
            printf("Error while opening %s\nTerminating execution...", pathname_A);
            return -2;
        }

        while((dp = readdir(dir)) != NULL) {
            if(strcmp(dp->d_name, curr_dir) == 0 || strcmp(dp->d_name, prv_dir) == 0)
                continue;

            if(recursiveCopy(pathname_A, pathname_B, dp->d_name, level+1) == -1)
                return -1;
        }
    }
    else{
        copyFile(pathname_A, pathname_B);
    }

    return 0;
}

int checkIfDirectory(char *path){

    int retValue;           // 0 - Okay; -1 - Error
    struct stat buf;

    retValue = stat(path, &buf);
    if(retValue == -1 || S_ISDIR(buf.st_mode) == 0)
        retValue = -1;
    else
        retValue = 0;

    return retValue;
}

int createDirectory(char *path){

    int retValue;       // 0 - Okay; -1 - Error
    struct stat buf;

    if(checkIfDirectory(path) == -1)
        retValue = mkdir(path);
        // Linux version
        //retValue = mkdir(path, (S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH) );
    else
        retValue = 0;

    return retValue;
}

int copyFile(char *path, char *path_copy){

    int src_fd, dest_fd;
    int elem_letti;
    char buffer[MAX_CHAR];


    src_fd = open(path, O_RDONLY);
    if(src_fd == -1)
        return -1;

    dest_fd = open(path_copy, O_CREAT | O_WRONLY);
    if(dest_fd == -1)
        return -1;

    while((elem_letti = read(src_fd, buffer, MAX_CHAR)) > 0)
        write(dest_fd, buffer, elem_letti);

    return 0;
}
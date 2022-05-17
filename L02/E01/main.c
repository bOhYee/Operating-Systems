#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#define MAX_PATH 150+1
#define MAX_CHAR 50+1

const char curr_dir[MAX_CHAR] = ".";
const char prv_dir[MAX_CHAR] = "..";

int getInfo(char *file_name, char *path, int level);
void getTypeOfFile(struct stat fileInfo, char type[]);

int main(int argc, char **argv) {

    int state;
    struct stat ibuf;

    if(argc < 2){
        printf("Inserire il path (ass. o rel.) di cui visualizzare il contenuto.\n");
        return -1;
    }

    state = stat(argv[1], &ibuf);
    if(state == -1 || S_ISDIR(ibuf.st_mode) == 0) {
        printf("Arguments not valid!\n");
        return -1;
    }

    getInfo("", argv[1], 0);
    return 0;

}

int getInfo(char *file_name, char *path, int level){

    int len;
    char type[MAX_CHAR] = "";
    char new_path[MAX_CHAR] = "";

    struct stat buf;
    struct dirent *dp;
    DIR *dir;

    len = (int) strlen(path);
    stat(path, &buf);
    if(S_ISDIR(buf.st_mode)){
        if(!(path[len-1] == 47 || path[len-1] == 92))
            strcat(path, "/");

        printf("Opening directory: %s\n", path);
        dir = opendir(path);
        if(dir == NULL) {
            printf("Error while opening %s\nTerminating execution...", file_name);
            return -1;
        }

        printf("%s\n%s\n", curr_dir, prv_dir);
        while((dp = readdir(dir)) != NULL) {
            if(strcmp(dp->d_name, curr_dir) == 0 || strcmp(dp->d_name, prv_dir) == 0)
                continue;

            strcpy(new_path, "");
            strcat(new_path, path),
            strcat(new_path, dp->d_name);
            if(getInfo(dp->d_name, new_path, level+1) == -1)
                return -1;
        }
        printf("Closing directory: %s\n", file_name);
    }
    else{
        getTypeOfFile(buf, type);
        printf("%d\t%s\t%s\n", (int) buf.st_size, file_name, type);
    }

    return 0;
}

void getTypeOfFile(struct stat fileInfo, char type[]){

    if(S_ISBLK(fileInfo.st_mode)){
        strcpy(type, "block special file");
    }
    else if(S_ISCHR(fileInfo.st_mode)){
        strcpy(type, "character special file");
    }
    else if(S_ISREG(fileInfo.st_mode)){
        strcpy(type, "regular file");
    }
    else if(S_ISFIFO(fileInfo.st_mode)){
        strcpy(type, "pipe file");
    }

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define FD_PIPE 2
#define MAX_CHAR 100+1

void P1(int *p1, int *p2);
void P2(int *p1, int *p2);

int main(){

    int p1[FD_PIPE], p2[FD_PIPE];
    int n;
    pid_t proc_1, proc_2;

    // Creation of two pipes
    if(pipe(p1) == -1){
        printf("Error during creation of a pipe.\n");
        return -1;
    }

    if(pipe(p2) == -1){
        printf("Error during creation of a pipe.\n");
        return -1;
    }

    // Creation of two processes
    proc_1 = fork();
    if(proc_1 == -1){
        printf("Error while forking.\n");
        return -1;
    }
    else if(proc_1 == 0){
        P1(p1, p2);
    }
    else{
        proc_2 = fork();
        if(proc_2 == -1){
            printf("Error while forking.\n");
            return -1;
        }
        else if(proc_2 == 0){
            P2(p1, p2);
        }
    }

    wait(NULL);
    wait(NULL);

    return 0;
}

void P1(int *p1, int *p2){

    int n, len;
    char buff[MAX_CHAR];

    // Child one reads from standard input and sends the data to child two
    printf("Digit a number: ");
    scanf("%d", &n);

    if (write(p1[1], &n, sizeof(int)) != sizeof(int)){
        printf("Error while P1 was writing on Pipe #1.");
        exit(1);
    }

    // Child one has to read and print what child two has written on the pipe
    for(int i = 0; i < n; i++) {
        // Read the length of the string
        if (read(p2[0], &len, sizeof(int)) != sizeof(int)) {
            printf("Error while P1 was reading from Pipe #2.\n");
            exit(1);
        }

        if (read(p2[0], buff, len) != len) {
            printf("Error while P1 was reading from Pipe #2.\n");
            exit(1);
        }

        printf("Received: %s\n", buff);
    }

}

void P2(int *p1, int *p2){

    int n, len;
    char buff[MAX_CHAR];

    // Child two needs to read how many strings it need to write on pipe two
    if (read(p1[0], &n, sizeof(int)) != sizeof(int)){
        printf("Error while P2 was reading on Pipe #1.");
        exit(1);
    }

    for(int i = 0; i < n; i++){
        printf("Digit a string: ");
        scanf("%s", buff);

        len = strlen(buff);
        if(write(p2[1], &len, sizeof(int)) != sizeof(int)){
            printf("Error while P2 was writing on Pipe #2.\n");
            exit(1);
        }

        if(write(p2[1], buff, len) != len){
            printf("Error while P2 was writing on Pipe #2.\n");
            exit(1);
        }
    }
}

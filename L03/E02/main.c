#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

typedef enum{
    EXEC, N, T
} arguments;

int convertStrToInt(char *str);
void recursiveFork(int n);

int main(int argc, char **argv) {

    int n, sleep_time;
    int f_n_proc = 0;

    // Check if n and t were defined with the command
    if(argc < 3){
        printf("Error: missing arguments!\n");
        return -1;
    }

    // Recover arguments and convert them into integer
    n = convertStrToInt(argv[N]);
    sleep_time = convertStrToInt(argv[T]);
    if(n == -1 || sleep_time == -1){
        printf("Error: parameters are not well-defined!\n");
        return -1;
    }

    // Start creating processes
    if(f_n_proc == 0)
	    printf("S-Starting process: \tPID = %d, \tPPID=%d\n", getpid(), getppid());

    recursiveFork(n);
    printf("Z-Leaf sleeping for %d seconds: \tPID=%d, \tPPID=%d\n", sleep_time, getpid(), getppid());
    sleep(sleep_time);
    printf("Z-Leaf terminating: \tPID=%d, \tPPID=%d\n", getpid(), getppid());
    printf("Z-Leaf terminating: \tPID=%d, \tPPID=%d\n", getpid(), getppid());

    return 0;
}

void recursiveFork(int n){

    pid_t pid;

    if(n > 0){
        pid = fork();
        if(pid == -1){
            printf("E-Error while creating process!\n");
            exit(1);
        }

        if(pid != 0){
            pid = fork();
            if(pid == -1){
                printf("E-Error while creating process!\n");
                exit(1);
            }

            if(pid != 0) {
                printf("X-Terminating parent: \tPID = %d, \tPPID=%d\n", getpid(), getppid());
                exit(0);
            }
        }
        recursiveFork(n-1);
    }

}

int convertStrToInt(char *str){

    int value;
    char *endptr = NULL;

    value = (int) strtol(str, &endptr, 10);
    if(str == endptr || (value == 0 && errno != 0) || value < 0)
        return -1;

    return value;
}

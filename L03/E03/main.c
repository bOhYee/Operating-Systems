#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printSP(int i);
pid_t forkForPrint();

int main() {

    pid_t pid;

    printSP(1);
    pid = forkForPrint();

    if(pid == 0){
        printSP(3);
        pid = forkForPrint();

        if(pid == 0){
            printSP(6);
            exit(0);
        }
        else{
            wait((int*) 0);
            printSP(8);
        }
    }
    else{
        printSP(2);
        pid = forkForPrint();

        if(pid == 0){
            printSP(5);
            exit(0);
        }
        else{
            printSP(4);
            wait((int*) 0);
        }

        printSP(7);
        wait((int*) 0);
        printSP(9);
    }

    return 0;
}

void printSP(int i){
    printf("Starting P%d\tPID=%d\tPPID=%d\n", i, getpid(), getppid());
}

pid_t forkForPrint(){

    pid_t pid;

    pid = fork();
    if(pid == -1){
        printf("Error while forking!\n");
        exit(1);
    }

    return pid;
}

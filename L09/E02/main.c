#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define N_SEM 6
#define S_READ 0
#define S_WRITE 1

typedef enum{
    S1, S2, S3, S4, S5, S6
} SEMAPHORES;

void semInit(int **sem, int n_value);
void semWait(int *sem);
void semSignal(int *sem);
void semDelete(int *sem);

int main(){

    int *sem[N_SEM];
    pid_t pid;

    for(int i = 0; i < N_SEM; i++)
        semInit(&sem[i], 0);

    // Best lazy solution
    while(1){
        printf("Start of a new cycle:\n");
        pid = fork();

        // Process A
        if(pid == 0){
            printf("Process A running...\n");

            for(int i = 0; i < 3; i++)
                semSignal(sem[S1]);

            break;
        }
        else{
            pid = fork();
            // Process B
            if(pid == 0) {
                semWait(sem[S1]);
                printf("Process B running...\n");
                semSignal(sem[S2]);
                break;
            }
            else{
                pid = fork();
                // Process C
                if(pid == 0){
                    pid = fork();
                    // Process E
                    if(pid == 0){
                        semWait(sem[S3]);
                        printf("Process E running...\n");
                        semSignal(sem[S4]);
                        break;
                    }
                    else{
                        pid = fork();
                        // Process F
                        if(pid == 0){
                            semWait(sem[S3]);
                            printf("Process F running...\n");
                            semSignal(sem[S4]);
                            break;
                        }
                        else{
                            pid = fork();
                            // Process C
                            if(pid == 0){
                                semWait(sem[S1]);
                                printf("Process C running...\n");
                                semSignal(sem[S3]);
                                semSignal(sem[S3]);
                                break;
                            }
                            else{
                                // Process G
                                semWait(sem[S4]);
                                semWait(sem[S4]);
                                printf("Process G running...\n");
                                semSignal(sem[S2]);
                                break;
                            }
                        }
                    }
                }
                else{
                    pid = fork();
                    // Process D
                    if(pid == 0){
                        pid = fork();
                        // Process D
                        if(pid == 0){
                            semWait(sem[S1]);
                            printf("Process D running...\n");
                            semSignal(sem[S5]);
                            break;
                        }
                        else{
                            // Process H
                            semWait(sem[S5]);
                            printf("Process H running...\n");
                            semSignal(sem[S2]);
                            break;
                        }
                    }
                    else{
                        pid = fork();
                        // Process I
                        if(pid == 0){
                            semWait(sem[S2]);
                            semWait(sem[S2]);
                            semWait(sem[S2]);
                            printf("Process I running...\n");
                            semSignal(sem[S6]);
                            break;
                        }
                        else{
                            semWait(sem[S6]);
                            printf("Cycle completed!\n\n");
                         }
                    }
                }
            }
        }


    }

    return 0;
}

void semInit(int **sem, int n_value){

    char toWrite = 'x';

    *sem = (int *) malloc(2 * sizeof(int));
    if(*sem == NULL){
        printf("Error during allocation!\n");
        exit(1);
    }

    if(pipe(*sem) == -1){
        printf("Error during creation of a pipe!\n");
        exit(1);
    }

    for(int i = 0; i < n_value; i++){
        if(write((*sem)[S_WRITE], &toWrite, 1) != 1){
            printf("Error during write on a pipe!\n");
            exit(1);
        }
    }

}

void semWait(int *sem){

    char toRead;

    if(read(sem[S_READ], &toRead, 1) != 1){
        printf("Error during read of a pipe!\n");
        exit(1);
    }

}

void semSignal(int *sem){

    char toWrite = 'x';

    if(write(sem[S_WRITE], &toWrite, 1) != 1){
        printf("Error during write on a pipe!\n");
        exit(1);
    }

}

void semDelete(int *sem){

    free(sem);

}
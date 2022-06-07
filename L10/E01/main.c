#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#define NUM_THREADS 3
#define MAX_LINE 100+1

sem_t sem_1, sem_2, sem_3;

typedef enum{
    EXEC, STRIN, STROUT
} ARGS;

typedef struct{
    FILE *fin;
    FILE *fout;
    int finished;
    char buff[MAX_LINE];
} FileLine;

void *readFile(void *args);
void *reverseLine(void *args);
void *writeFile(void *args);

/*
 * Semaphores guideline
 * sem_1 : T1 -> T2
 * sem_2 : T2 -> T3
 * sem_3 : T3 -> T1
 */

int main(int argc, char **argv){

    FILE *fin, *fout;
    FileLine curr;
    pthread_t p_id[NUM_THREADS];

    // Check the number of args
    if(argc < 3){
        printf("Error: not enough arguments passed to the program!\n");
        return -1;
    }

    // Open the input file
    fin = NULL;
    fin = fopen(argv[STRIN], "r");
    if(fin == NULL){
        printf("Error: input file does not exit!\n");
        return -1;
    }

    // Open the output file
    fout = NULL;
    fout = fopen(argv[STROUT], "w");
    if(fout == NULL){
        printf("Error: output file cannot be created!\n");
        return -1;
    }

    // Initialize the values for the struct shared between the threads
    curr.fin = fin;
    curr.fout = fout;
    curr.finished = 0;
    strcpy(curr.buff, "");

    // Init of the semaphores
    sem_init(&sem_1, 0, 0);
    sem_init(&sem_2, 0, 0);
    sem_init(&sem_3, 0, 1);

    pthread_create(&p_id[0], NULL, readFile, (void *) &curr);
    pthread_create(&p_id[1], NULL, reverseLine, (void *) &curr);
    pthread_create(&p_id[2], NULL, writeFile, (void *) &curr);

    // Waiting for the children to finish
    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(p_id[i], NULL);

    // Deallocating the semaphores before closing the main process
    sem_destroy(&sem_1);
    sem_destroy(&sem_2);
    sem_destroy(&sem_3);

    return 0;
}

void *readFile(void *args){

    FileLine *curr;

    curr = (FileLine *) args;

    while(1) {
        // Wait until task 3 has finished writing on the output file
        sem_wait(&sem_3);
        if (fgets(curr->buff, MAX_LINE, curr->fin) == NULL) {
            curr->finished = 1;
            sem_post(&sem_1);
            printf("Read terminated.\nExiting...\n\n");
            pthread_exit(NULL);
        }
        else{
            printf("Read string: %s", curr->buff);
            sem_post(&sem_1);
        }
    }

}

void *reverseLine(void *args){

    int l;
    char temp[MAX_LINE];
    FileLine *curr;

    curr = (FileLine *) args;
    while(1){

        // Wait until T1 has read a line or has concluded its cycle
        sem_wait(&sem_1);

        if(curr->finished == 1) {
            sem_post(&sem_2);
            pthread_exit(NULL);
        }

        // Reverse the string
        l = strlen(curr->buff) - 2;
        for(int i = 0; i < MAX_LINE; i++)
            temp[i] = toupper(curr->buff[l--]);

        strcpy(curr->buff, temp);
        printf("String elaborated!\nNew string: %s\n", curr->buff);
        sem_post(&sem_2);
    }

}

void *writeFile(void *args){

    FileLine *curr;

    curr = (FileLine *) args;

    while(1){

        // Wait until T2 has reversed the string
        sem_wait(&sem_2);

        if(curr->finished == 1)
            pthread_exit(NULL);

        // Write the string on output file
        printf("Writing on file the string...\n\n");
        fprintf(curr->fout, "%s\n", curr->buff);
        sem_post(&sem_3);
    }

}
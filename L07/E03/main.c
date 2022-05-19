#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#define CHAR_SIZE 50+1
#define MAX_N_PER_FILE 15

typedef enum{
    EXEC, N, IN, OUT
} params;

typedef struct{
    char *strFile;
    int *vect;
} arg_t;

void allocateMString(char ***matString, int numOfString);
void allocateMVect(int ***fileNumbers, int numOfFiles);
void freeMString(char **matString, int numOfString);
void freeMVect(int **fileNumbers, int numOfFiles);
void associateNameFile(char **matString, int numOfString, char *baseFileName);
void *threadRoutine(void *arg);
void bubble_sort(int *vect, int totNum);
int mergeVect(int *totVect, int *numToAdd, int sp);

int main(int argc, char **argv){

    int totFile, resC, currPos, *status_t;
    int *totNumbers;
    int **fileNumbers;
    char **inFileName;
    pthread_t *tid;
    arg_t **threadArgs;

    // Check if arguments are passed to the program
    if(argc < 4){
        printf("Not enough arguments!\n");
        exit(1);
    }

    totFile = atoi(argv[N]);

    // Allocating the two matrix of strings and the vector containing the thread ids
    allocateMVect(&fileNumbers, totFile);
    allocateMString(&inFileName, totFile);
    associateNameFile(inFileName, totFile, argv[IN]);           // Creating the name of the input files

    tid = (pthread_t *) malloc(totFile * sizeof(pthread_t));
    if(tid == NULL){
        printf("Error during allocation!\n");
        exit(1);
    }

    threadArgs = (arg_t **) malloc(sizeof(arg_t*));             // I also need a vector to maintain the pointers to the thread structures
    if(threadArgs == NULL){
        printf("Error during allocation!\n");
        exit(1);
    }

    totNumbers = (int *) malloc(totFile * MAX_N_PER_FILE * sizeof(int));
    if(totNumbers == NULL){
        printf("Error during allocation!\n");
        exit(1);
    }

    // Defining the threads
    for(int i = 0; i < totFile; i++){
        threadArgs[i] = (arg_t *) malloc(sizeof(arg_t));
        if(threadArgs[i] == NULL){
            printf("Error during allocation!\n");
            exit(1);
        }

        threadArgs[i]->strFile = inFileName[i];
        threadArgs[i]->vect = fileNumbers[i];

        resC = pthread_create(&tid[i], NULL, threadRoutine, (void *) threadArgs[i]);
        if(resC != 0){
            printf("Error while creating a thread!\nError code: %d\n", resC);
            exit(1);
        }
    }

    // Waiting for thread termination
    currPos = 0;
    for(int i = 0; i < totFile; i++){
        resC = pthread_join(tid[i], (void **) &(status_t));
        printf("Error during execution of thread %d.\nTerminatation status: %d\n", i, status_t);
        /*if(resC != 0 || ((*status_t) != 0)){
            printf("Error during execution of thread %d.\nTerminatation status: %d\n", i, (*status_t));
            exit(1);
        }*/

        // Merging
        currPos = mergeVect(totNumbers, threadArgs[i]->vect, currPos);
    }

    // Printing
    printf("\nResult:\n");
    for(int i = 0; i < currPos; i++)
        printf("%d ", totNumbers[i]);

    printf("\n");

    // Deallocating memory
    freeMString(inFileName, totFile);
    freeMVect(fileNumbers, totFile);
    free(tid);

    for(int i = 0; i < totFile; i++)
        free(threadArgs[i]);

    free(threadArgs);
    free(totNumbers);

    return 0;
}

void allocateMString(char ***matString, int numOfString){

    *matString = (char **) malloc(numOfString * sizeof(char *));
    if(*matString == NULL){
        printf("Error during allocation!\n");
        exit(1);
    }

    for(int i = 0; i < numOfString; i++){
        (*matString)[i] = (char *) malloc(CHAR_SIZE * sizeof(char));
        if((*matString)[i] == NULL) {
            printf("Error during allocation!\n");
            exit(1);
        }
    }

}

void allocateMVect(int ***fileNumbers, int numOfFiles){

    *fileNumbers = (int **) malloc(numOfFiles * sizeof(int *));
    if(*fileNumbers == NULL){
        printf("Error during allocation!\n");
        exit(1);
    }

    for(int i = 0; i < numOfFiles; i++){
        (*fileNumbers)[i] = (int *) malloc(MAX_N_PER_FILE * sizeof(int));
        if((*fileNumbers)[i] == NULL) {
            printf("Error during allocation!\n");
            exit(1);
        }

        // I suppose that the numbers are gonna be positive
        for(int l = 0; l < MAX_N_PER_FILE; l++)
            (*fileNumbers)[i][l] = -1;
    }

}

void freeMString(char **matString, int numOfString){

    for(int i = 0; i < numOfString; i++)
        free(matString[i]);

    free(matString);
}

void freeMVect(int **fileNumbers, int numOfFiles){

    for(int i = 0; i < numOfFiles; i++)
        free(fileNumbers[i]);

    free(fileNumbers);
}

void associateNameFile(char **matString, int numOfString, char *baseFileName){

    for(int i = 0; i < numOfString; i++)
        sprintf(matString[i], "%s%d.txt", baseFileName, i+1);

}

void *threadRoutine(void *arg){

    int status, nRow;
    arg_t *threadArgs;
    FILE *fIn;

    threadArgs = (arg_t *) arg;
    status = 0;
    fIn = NULL;

    // Opening the two files
    fIn = fopen(threadArgs->strFile, "r");
    if(fIn == NULL){
        printf("Error while opening %s.\nError code: %d\n", threadArgs->strFile, errno);
        status = -1;
        pthread_exit(&status);
    }

    // Reading the input file and allocating a vector for ordering the numbers
    fscanf(fIn, "%d", &nRow);
    for(int i = 0; i < nRow; i++)
        fscanf(fIn, "%d", &threadArgs->vect[i]);

    bubble_sort(threadArgs->vect, nRow);
    fclose(fIn);

    printf("File sorted: %s\n", threadArgs->strFile);
    pthread_exit(&status);
}

void bubble_sort(int *vect, int totNum){

    // Definitely not copied :)
    int tmp;

    for (int i = 0; i < totNum; i++) {   // loop totNum times - 1 per element
        for (int j = 0; j < totNum - i - 1; j++) { // last i elements are sorted already
            if (vect[j] > vect[j + 1]) {  // swop if order is broken
                tmp = vect[j];
                vect[j] = vect[j + 1];
                vect[j + 1] = tmp;
            }
        }
    }

}

int mergeVect(int *totVect, int *numToAdd, int sp){

    for(int i = 0; (i < MAX_N_PER_FILE && numToAdd[i] != -1); i++)
        totVect[sp++] = numToAdd[i];

    return sp;
}
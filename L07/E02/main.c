#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#define CHAR_SIZE 50+1

typedef enum{
    EXEC, N, IN, OUT
} params;

typedef struct{
    char *strIn;
    char *strOut;
} arg_t;

void allocateMString(char ***matString, int numOfString);
void freeMString(char **matString, int numOfString);
void associateNameFile(char **matString, int numOfString, char *baseFileName);
void *threadRoutine(void *arg);
void bubble_sort(int *vect, int totNum);

int main(int argc, char **argv){

    int totFile, resC, *status_t;
    char **inFileName, **outFileName;
    pthread_t *tid;
    arg_t *threadArgs;

    // Check if arguments are passed to the program
    if(argc < 4){
        printf("Not enough arguments!\n");
        exit(1);
    }

    totFile = atoi(argv[N]);

    // Allocating the two matrix of strings and the vector containing the thread ids
    allocateMString(&inFileName, totFile);
    allocateMString(&outFileName, totFile);
    tid = (pthread_t *) malloc(totFile * sizeof(pthread_t));
    if(tid == NULL){
        printf("Error during allocation!\n");
        exit(1);
    }

    // Creating the name of the input and output files
    associateNameFile(inFileName, totFile, argv[IN]);
    associateNameFile(outFileName, totFile, argv[OUT]);

    // Defining the threads
    for(int i = 0; i < totFile; i++){
        threadArgs = (arg_t *) malloc(sizeof(arg_t));
        if(threadArgs == NULL){
            printf("Error during allocation!\n");
            exit(1);
        }

        threadArgs->strIn = inFileName[i];
        threadArgs->strOut = outFileName[i];

        resC = pthread_create(&tid[i], NULL, threadRoutine, (void *) threadArgs);
        if(resC != 0){
            printf("Error while creating a thread!\nError code: %d\n", resC);
            exit(1);
        }
    }

    status_t = NULL;
    for(int i = 0; i < totFile; i++){
        resC = pthread_join(tid[i], (void **) (&status_t));
        if(resC != 0 || *status_t != 0){
            printf("Error during execution of thread %d.\nTerminatation status: %d\n", i, (*status_t));
            exit(1);
        }
    }

    // Deallocating memory
    freeMString(inFileName, totFile);
    freeMString(outFileName, totFile);

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

void freeMString(char **matString, int numOfString){

    for(int i = 0; i < numOfString; i++)
        free(matString[i]);

    free(matString);
}

void associateNameFile(char **matString, int numOfString, char *baseFileName){

    for(int i = 0; i < numOfString; i++)
        sprintf(matString[i], "%s%d.txt", baseFileName, i+1);

}

void *threadRoutine(void *arg){

    int status, nRow;
    int *vect;
    arg_t *threadArgs;
    FILE *fIn, *fOut;

    threadArgs = (arg_t *) arg;
    fIn = NULL;
    fOut = NULL;
    vect = NULL;

    // Opening the two files
    fIn = fopen(threadArgs->strIn, "r");
    if(fIn == NULL){
        printf("Error while opening %s.\nError code: %d\n", threadArgs->strIn, errno);
        status = -1;
        pthread_exit((void *) (&status));
    }

    fOut = fopen(threadArgs->strOut, "w");
    if(fOut == NULL){
        printf("Error while opening %s.\nError code: %d\n", threadArgs->strOut, errno);
        status = -1;
        pthread_exit((void *) (&status));
    }

    // Reading the input file and allocating a vector for ordering the numbers
    fscanf(fIn, "%d", &nRow);
    vect = (int *) malloc(nRow * sizeof(int));
    if(vect == NULL){
        printf("Error while allocating vector to order file numbers!\n");
        pthread_exit((void *) (&status));
    }

    for(int i = 0; i < nRow; i++)
        fscanf(fIn, "%d", &vect[i]);

    bubble_sort(vect, nRow);
    for(int i = 0; i < nRow; i++)
        fprintf(fOut, "%d\n", vect[i]);

    printf("File copied: %s\n", threadArgs->strIn);
    fclose(fIn);
    fclose(fOut);
    free(threadArgs);

    status = 0;
    pthread_exit((void *) (&status));
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
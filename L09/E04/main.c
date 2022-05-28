#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define NUM_SEM 6
#define NUM_THREAD 9

void *pA(void *arg);
void *pB(void *arg);
void *pC(void *arg);
void *pD(void *arg);
void *pE(void *arg);
void *pF(void *arg);
void *pG(void *arg);
void *pH(void *arg);
void pI();

typedef enum{
    A, B, C, D, E, F, G, H
} THREADS;

// Same type of management of exercise L09/E02
typedef enum{
    S1, S2, S3, S4, S5, S6
} SEMAPHORES;

// Global variable so that every thread can see the semaphores
sem_t sem_id[NUM_SEM];

int main(){

    int i;
    pthread_t pt_id[NUM_THREAD];

    // Initialization of the semaphores
    for(i = 0; i < (NUM_SEM-1); i++)
        sem_init(&sem_id[i], 0, 0);

    // S6 must be available to start task A
    sem_init(&sem_id[i], 0, 1);

    // Creating the threads
    pthread_create(&pt_id[A], NULL, pA, NULL);
    pthread_create(&pt_id[B], NULL, pB, NULL);
    pthread_create(&pt_id[C], NULL, pC, NULL);
    pthread_create(&pt_id[D], NULL, pD, NULL);
    pthread_create(&pt_id[E], NULL, pE, NULL);
    pthread_create(&pt_id[F], NULL, pF, NULL);
    pthread_create(&pt_id[G], NULL, pG, NULL);
    pthread_create(&pt_id[H], NULL, pH, NULL);

    pI();

    // Destroying the semaphores
    for(int l = 0; l < NUM_SEM; l++)
        sem_destroy(&sem_id[l]);

    return 0;
}

void *pA(void *arg){

    while(1) {
        sem_wait(&sem_id[S6]);
        printf("Process A running...\n");
        sem_post(&sem_id[S1]);
        sem_post(&sem_id[S1]);
        sem_post(&sem_id[S1]);
    }

    pthread_exit(NULL);
}

void *pB(void *arg){

    while(1) {
        sem_wait(&sem_id[S1]);
        printf("Process B running...\n");
        sem_post(&sem_id[S2]);
    }

    pthread_exit(NULL);
}

void *pC(void *arg){

    while(1) {
        sem_wait(&sem_id[S1]);
        printf("Process C running...\n");
        sem_post(&sem_id[S3]);
        sem_post(&sem_id[S3]);
    }

    pthread_exit(NULL);
}

void *pD(void *arg){

    while(1) {
        sem_wait(&sem_id[S1]);
        printf("Process D running...\n");
        sem_post(&sem_id[S5]);
    }

    pthread_exit(NULL);
}

void *pE(void *arg){

    while(1) {
        sem_wait(&sem_id[S3]);
        printf("Process E running...\n");
        sem_post(&sem_id[S4]);
    }

    pthread_exit(NULL);
}

void *pF(void *arg){

    while(1) {
        sem_wait(&sem_id[S3]);
        printf("Process F running...\n");
        sem_post(&sem_id[S4]);
    }

    pthread_exit(NULL);
}

void *pG(void *arg){

    while(1) {
        sem_wait(&sem_id[S4]);
        sem_wait(&sem_id[S4]);
        printf("Process G running...\n");
        sem_post(&sem_id[S2]);
    }

    pthread_exit(NULL);
}

void *pH(void *arg){

    while(1) {
        sem_wait(&sem_id[S5]);
        printf("Process H running...\n");
        sem_post(&sem_id[S2]);
    }

    pthread_exit(NULL);
}

void pI(){

    while(1) {
        sem_wait(&sem_id[S2]);
        sem_wait(&sem_id[S2]);
        sem_wait(&sem_id[S2]);
        printf("Process I running...\n");
        sem_post(&sem_id[S6]);
    }

}
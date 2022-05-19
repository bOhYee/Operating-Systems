#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void binary(int *binary_num, int nbit, int cbit);

int main(int argc, char **argv){

    int nbit;
    int *vet;

    // Check if arguments are passed to the program
    if(argc < 2){
        printf("Not enough arguments!\n");
        exit(1);
    }

    // Recover the number of bit required and use it to allocate the binary number vector
    nbit = atoi(argv[1]);
    vet = (int *) malloc(nbit * sizeof(int));
    if (vet == NULL) {
        printf("Allocatin Error.\n");
        exit(1);
    }

    // Start the generation of numbers
    printf("Binary Numbers:\n");
    binary(vet, nbit, 0);
    free(vet);

    return 0;
}

void binary(int *binary_num, int nbit, int cbit){

    if (cbit >= nbit) {
        for (int j = 0; j < nbit; j++)
            printf("%d", binary_num[j]);

        printf("\n");
        exit(0);
    }

    // Father puts 0 while it's child puts 1
    if(fork()){
        binary_num[cbit] = 0;
        binary(binary_num, nbit, cbit + 1);
    }
    else{
        binary_num[cbit] = 1;
        binary(binary_num, nbit, cbit + 1);
    }

    return;
}
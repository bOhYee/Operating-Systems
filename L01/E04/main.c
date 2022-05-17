#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define MAX_CHAR 100+1
#define MAX_BUFF 1000

void copiaTesto(FILE *src, FILE *dest);
void copiaBinario(FILE *src, FILE *dest);
void copiaSyscall(int src, int dest);

typedef enum {
    EXEC, MODE, SRC, DEST, TYPE
} arguments;

int main(int argc, char **argv) {

    FILE *src, *dest;
    int src_sys, dest_sys;

    src = NULL;
    dest = NULL;

    // Verifica argomenti
    if(argc < 4){
        printf("Errore: gli argomenti inseriti sono meno di quanto richiesto per lo svolgimento della copia!\n");
        return -1;
    }


    switch(strtol(argv[MODE], NULL, 10)){
        case 1:
            // Apro il file sorgente
            src = fopen(argv[SRC], "r");
            if(src == NULL){
                printf("Errore: impossibile aprire il file sorgente!\n");
                return -1;
            }

            // Apro/Creo il file di destinazione
            dest = fopen(argv[DEST], "w+");
            if(dest == NULL){
                printf("Errore: impossibile aprire il file di destinazione!\n");
                return -1;
            }

            switch(argv[TYPE][0]){
                case 'T':
                    copiaTesto(src, dest);
                    break;
                case 'B':
                    copiaBinario(src, dest);
                    break;
            }

            fclose(src);
            fclose(dest);
            break;
        case 2:
            // Apro il file sorgente
            src_sys = open(argv[SRC], O_RDONLY);
            if(src_sys == -1){
                printf("Errore: impossibile aprire il file sorgente tramite syscall!\n");
                return -1;
            }

            // Apro/Creo il file di destinazione
            dest_sys = open(argv[DEST], O_WRONLY);
            if(dest_sys == -1){
                printf("Errore: impossibile aprire il file di destinazione tramite syscall!\n");
                return -1;
            }

            copiaSyscall(src_sys, dest_sys);
            close(src_sys);
            close(dest_sys);
            break;
    }

    printf("Copia terminata!\n");
    return 0;
}

void copiaTesto(FILE *src, FILE *dest){

    char strToCopy[MAX_CHAR];

    while(fgets(strToCopy, MAX_CHAR, src) != NULL)
        fputs(strToCopy, dest);

}

void copiaBinario(FILE *src, FILE *dest){

    int elem_letti = 0;
    char buffer[MAX_BUFF];

    while((elem_letti = fread(buffer, 1, MAX_BUFF, src)) != 0)
        fwrite(buffer, 1, elem_letti, dest);

}

void copiaSyscall(int src, int dest){

    int elem_letti = 0;
    char buffer[MAX_BUFF];

    while((elem_letti = read(src, buffer, MAX_BUFF)) > 0)
        write(dest, buffer, elem_letti);

}
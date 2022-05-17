#include <stdio.h>

int main(){

    int num = 0;
    FILE *out = NULL;
    setbuf(stdout, 0);
    setbuf(stderr, 0);

    do{
        printf("Digita un numero: ");
        scanf("%d", &num);

        if(num != 0) {
            if (num % 2 == 0)
                out = stdout;
            else
                out = stderr;

            fprintf(out, "%d\n", num);
        }
    }while(num != 0);

    return 0;
}
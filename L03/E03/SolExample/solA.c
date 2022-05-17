/*
 * Precedence graph with fork and wait.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void pLeft (void);
void pRight (void);

int
main()
{
    pid_t pid;

    fprintf (stdout, "P1 (pid=%d)\n", getpid());
    pid = fork();
    if (pid == 0) {
        //sleep (1);
        pRight ();
        exit (0);
    } else {
        //sleep (1);
        pLeft ();
        wait ((int *) 0);
        fprintf (stdout, "P9 (pid=%d)\n", getpid());
        return (0);
    }
}

void
pLeft () {
    pid_t pid;

    fprintf (stdout, "P2 (pid=%d)\n", getpid());
    pid = fork();
    if (pid == 0) {
        //sleep (1);
        fprintf (stdout, "P5 (pid=%d)\n", getpid());
        exit (0);
    } else {
        //sleep (1);
        fprintf (stdout, "P4 (pid=%d)\n", getpid());
        while (wait ((int *) 0) != pid);
        fprintf (stdout, "P7 (pid=%d)\n", getpid());
        return;
    }
}

void
pRight () {
    pid_t pid;

    fprintf (stdout, "P3 (pid=%d)\n", getpid());
    pid = fork();
    if (pid == 0) {
        //sleep (1);
        fprintf (stdout, "P6 (pid=%d)\n", getpid());
        exit (0);
    } else {
        //sleep (1);
        wait ((int *) 0);
        fprintf (stdout, "P8 (pid=%d)\n", getpid());
        exit (0);
    }
}

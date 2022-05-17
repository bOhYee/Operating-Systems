#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main (
        int argc,
        char *argv[]
)
{
    pid_t pid1;
    pid_t pid2;
    int i;
    int n = atoi (argv[1]);
    int t = atoi (argv[2]);
    int counter=0;

    fprintf(stdout, "PORCODIO PID=%d; PPID=%d; counter=%d\n",
            getpid(), getppid(),counter);

    for (i=0; i<n; i++) {
        pid1 = fork();
        if (pid1 == 0){
            // Child1
            counter++;
            fprintf(stdout, "PID=%d; PPID=%d; counter=%d\n",
                    getpid(), getppid(),counter);
        } else {
            //Parent
            pid2 = fork();
            if (pid2 == 0){
                // Child2
                counter++;
                fprintf(stdout, "PID=%d; PPID=%d; counter=%d\n",
                        getpid(), getppid(),counter);
            } else {
                //Parent
                fprintf(stdout, "Process %d exiting\n", getpid());
                exit(0);// Stop Parent
            }
        }
    }

    fprintf(stdout, "PID %d. Leaf. Sleep for %d secs.\n",
            getpid(),t);
    sleep(t);

    return (0);
}

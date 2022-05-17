#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

void createChildren(int n)
{
    int pid=0;
    if (n) {
        pid=fork();
        if (pid!=0) {
            pid=fork();
            if (pid!=0)
                exit(0);
        }
        createChildren(n-1);
    }
    return;
}

int main (
        int argc,
        char** argv
)
{
    int n,t;
    n=atoi(argv[1]);
    t=atoi(argv[2]);
    createChildren(n);
    printf("\nLeaf %d - sleep %d seconds\n",getpid(),t);
    sleep(t);
    printf("\nLeaf %d - END\n",getpid());
    exit(0);
}

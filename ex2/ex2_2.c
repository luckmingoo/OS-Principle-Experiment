#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    pid_t fpid1;
    fpid1=fork();
    int status;
    if (fpid1 < 0)
        printf("error in fork!");   
    else if (fpid1 == 0) {  
        printf("child1 process id is %d\n",getpid());
        execl("/bin/ls","ls","-l",NULL);
        printf("child1\n");
        exit(0);
    }
    else {
       waitpid(fpid1, &status, 0 );
       printf("parent process id is %d\tchild1 process id is %d\n",getpid(),fpid1);
       printf("child1 process exited with status %d \n", status );
    } 
    return 0;
}

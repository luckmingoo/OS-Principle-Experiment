#include <unistd.h>  
#include <stdio.h>
int main()
{
    pid_t fpid1,fpid2;
    fpid1=fork();
    if (fpid1 < 0){
      printf("error in fork!");
    }
    else if (fpid1 == 0) {  
        printf("i am the child1 process, my process id is %d\n",getpid());   
    }
    else {
        printf("i am the parent process, my process id is %d\n",getpid());   
        fpid2=fork();
    if (fpid2 < 0){
        printf("error in fork!");
      }
      else if (fpid2 == 0) {  
        printf("i am the child2 process, my process id is %d\n",getpid());   
      }
      else {
        printf("i am the parent process, my process id is %d\n",getpid()); 
      }
    }
    
    return 0;  
}  

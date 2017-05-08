#include <unistd.h>  
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
  
int main()
{  
	int pipes[2];
	char data[] = "pipe communicate!";
  pid_t pid;  
  
	if(pipe(pipes) == 0)  
    {  
		pid = fork();  
   	if(pid == -1)  
        {	  
    	fprintf(stderr, "Fork failure!\n");  
     	exit(EXIT_FAILURE);  
       	}  
   	else if(pid == 0)  
        { 
			close(pipes[0]);//注意管道的工作方式为半双工 
			char commu_data[BUFSIZ];
			sprintf(commu_data,"%s child pid is %d",data,getpid());
			int nbytes = write(pipes[1],commu_data,strlen(commu_data));
			printf("child_%d - write %d bytes into pipe\n",getpid(),nbytes);           
			exit(EXIT_FAILURE);  
        }  
   	else  
        {
			int status;
			waitpid(pid, &status, 0 );
			close(pipes[1]);
			char readbuffer[BUFSIZ];
			int nbytes = read(pipes[0],readbuffer,sizeof(readbuffer));
     	close(pipes[0]); 
     printf("father_%d - read %d bytes contain \"%s\"\n", getpid(), nbytes, readbuffer); 
        } 
   	exit(EXIT_SUCCESS);
   	}
  else
	{
		fprintf(stderr,"pipe error!\n");
		exit(EXIT_FAILURE);
	}
}

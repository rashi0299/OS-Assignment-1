#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include <time.h>
#include<errno.h>

int main(int argc, char *argv[])
{
	int fds[2];
	pipe(fds);

	int pid=fork();

	if(pid==0)
	{
		//child process
		close(fds[0]);
		dup2(fds[1],1);

		char * args[] = {"grep",argv[1],"-n",argv[2],NULL};
		execvp(args[0],args);  
		
	}
	else
	{
		
		close(fds[1]);
		dup2(fds[0],0);
		// cut command 
		char * args[] = {"cut","-d",":","-f","1","-",NULL};
		execvp(args[0],args); 

	}
	return 0;
}
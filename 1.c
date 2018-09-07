#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include<errno.h>

void siginthandler(int signal)
{
    printf("\nType \"quit\" (without quotes) to exit\n");
}

int main()
{
    signal(SIGINT,siginthandler);
    int pid;

    while(1)
    {
        pid = 0;
        char currentD[FILENAME_MAX];
        getcwd(currentD,FILENAME_MAX);
        printf("rashi@rashi:%s$ ",currentD);
        char array[1000];            //stores the input
        char *finalArr[1000] = {NULL};       // stores after tok
        
        scanf("%[^\n]s",array);
        while ((getchar()) != '\n'); 
        
        char* token = strtok(array," ");

        int noOfTokens=0;
        
        while(token != NULL)
        {
            finalArr[noOfTokens++]=token;
            //printf("i=%d %s\n",noOfTokens-1,finalArr[noOfTokens-1]);
            token = strtok(NULL, " ");
        }
        if(strcmp(finalArr[0],"quit")==0)
        {
            if(pid!=0) kill(pid,SIGKILL);
            exit(0);
        }
        else if(strcmp(finalArr[0],"cd")==0 || strcmp(finalArr[0],"ls")==0 || strcmp(finalArr[0],"wc")==0 || strcmp(finalArr[0],"echo")==0)
        {

            if(strcmp(finalArr[0],"cd")==0)
            {
                if(finalArr[1] != NULL)
                {
                    if(chdir(finalArr[1])!=0)
                    {
                        printf("No such directory!\n");
                    }

                }
            }
            else if(strcmp(finalArr[0],"wc")==0)
            {
                pid = fork();
                {
                    if(pid==0)
                    {

                        char * args[] = {finalArr[0],finalArr[1],finalArr[2], NULL};
                        execvp(args[0],args);  
                        
                    }
                    else
                    {
                        int stat;
                        wait(&stat);
                    }
                }   
            }
            else if(strcmp(finalArr[0],"ls")==0)
            {
                pid = fork();
                if(pid==0)
                {
                    if(finalArr[1]!=NULL)
                    {
                        printf("finalArr[1] = %s\n",finalArr[1]);
                        char * args[] = {finalArr[0],finalArr[1],NULL};
                        execvp(args[0],args);  
                    }
                    else
                    {
                        printf("%s\n",currentD);
                        char * args[] = {finalArr[0],currentD,NULL};
                        execvp(args[0],args);  
                    }
                }
                else
                {
                    int stat;
                    wait(&stat);
                }
            }
            else
            {
                pid = fork();
                {
                    if(pid==0)
                    {
                        execvp(finalArr[0],finalArr);  
                    }
                    else
                    {
                        int stat;
                        wait(&stat);
                    }
                }   
            }
            
        }
        else
            printf("command doesn't exist\n");
    }
    return 0;
}
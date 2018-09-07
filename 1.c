#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include<errno.h>

int main()
{
    char currentD[FILENAME_MAX];
    getcwd(currentD,FILENAME_MAX);
    printf("%s",&currentD);
    
    while(1)
    {
        //printf("rashi@rashi:~$");
        char array[1000];            //stores the input
        char *finalArr[1000];       // stores after tok
        
        scanf("%[^\n]s",array);
        printf("%s\n",&array);
        
        char* token = strtok(array," ");
        int noOfTokens=0;
        
        while(token != NULL)
        {
            finalArr[noOfTokens++]=token;
            //printf("i=%d %s\n",noOfTokens-1,finalArr[noOfTokens-1]);
            token = strtok(NULL, " ");
        }
        if(strcmp(finalArr[0],"quit")==0)
            break;
        else if(strcmp(finalArr[0],"cd")==0 || strcmp(finalArr[0],"ls")==0 || strcmp(finalArr[0],"wc")==0 || strcmp(finalArr[0],"echo")==0)
        {
            int pid;
            pid=fork();
            if(pid==0)
            {
                if(strcmp(finalArr[0],"cd")==0)
                {
                    if(finalArr[1] != NULL)
                    {
                        strcat(currentD,finalArr[1]);
                    }
                }
                else if(strcmp(finalArr[0],"wc")==0)
                {
                    char temp[FILENAME_MAX];
                    strcpy(temp,currentD);
                    strcat(temp,finalArr[2]);
                    char * args[] = {finalArr[0],finalArr[1],temp};
                    execvp(args[0],args);  
                }
                else if(strcmp(finalArr[0],"ls")==0)
                {
                    char * args[] = {finalArr[0],currentD};
                    execvp(args[0],args);  
                }
                else
                    printf("%s",&finalArr[1]);
            }
            else
            {
                int status;
                wait(&status);
            }
            
        }
        else
            printf("command doesn't exist\n");
    }
    return 0;
}
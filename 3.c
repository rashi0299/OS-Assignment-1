#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include <time.h>
#include<errno.h>

int main()
{
    int pdf[2];
    pipe(pdf);
    int pid = fork();

    if(pid==0)
    {

        int j, s=0;
        int z;
        close(pdf[1]);

        //Child Process

        while(1)
        {
            read(pdf[0],&z,sizeof(z));  

            for(j=2;j<z;j++)
            {
                if(z%j == 0)
                {
                    break;
                }
            }

            if(j==z)
            {
                printf("%s\n","It is prime");
                s+=z;
            }
            else
            {
                printf("%s\n","It is composite");
            }
            printf("s = %d\n",s);

            sleep(z%3);
        }

    }
    else
    {
        //Parent Process
        close(pdf[0]);
        srand(time(NULL));      

        int arr[]={11,12,13,14,15,16,17,18,19,20};
        int choice[10] = {0}; 

        int count = 0;

        while(1)
        {

            int position = rand()%10;
            int x = arr[position];
            printf("x = %d and ",x);
            choice[position]=1;

            position =  rand() % 10;
            int y = arr[position];
            printf("y = %d\n",y);
            choice[position]=1;

            if(x!=y)
            {   
                write(pdf[1],&y,sizeof(int));           //changes

                sleep(x/3);
            }
            count = 0;
            for(int k=0;k<10;k++)
            {
                if(choice[k]==1) 
                    count++;
            }
            if(count==10) 
                break;
        }
        kill(pid,SIGKILL);
        exit(0);
    }

}
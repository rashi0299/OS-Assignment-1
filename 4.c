#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>

void my_handler(){

	printf(" Type 'quit' to exit\nShell:");
	fflush(stdout);
}

char* get_line(void){

	char* line= NULL;
	ssize_t buffer_size=0;
	getline(&line,&buffer_size,stdin);

	return line;
} 


char** get_tokens(char* line,int* size){
	char** tokens= (char**)malloc(100*sizeof(char*));
	char* token;
	while(token=strtok_r(line," ",&line)){
		tokens[(*size)++]=token;
	}
	tokens[(*size)-1][strlen(tokens[(*size)-1])-1]='\0';
	tokens[(*size)++]=(char*)NULL;

	return tokens;
}


void execute(char* path,char** args){

	int pid= fork();
	if(pid==0){
		char* cmd= (char*)malloc(1000);
		cmd[0]='\0';
		strcat(cmd,path);
		strcat(cmd,args[0]);
		if(execv(cmd,args)==-1) perror("Error in Command");
		exit(0);
	}else{
		wait(NULL);
		return;
	}
	return;
}


void my_cd(char** args,int size){

	if(size>3){
		perror("Number of arguments");
		return;
	}

	if(chdir(args[1])==-1){
		perror("File error");
	}
	else{
		char* args[2]={"pwd",(char*)NULL};
		return execute("/bin/",args);
	}

}

int my_wc(char** args){
	execute("/usr/bin/",args);
}

int main(){

	char quit[5];
	char check[5];
	strcpy(quit,"quit");
	signal(SIGINT,my_handler);
	int flag;
	while(1){
		flag=1;
		printf("Shell: ");
		char* line= get_line();
		if(line[0]=='\n') continue;
		int size=0;
		if(strlen(line)==5){
			strcpy(check,line);
			if(strncmp(check,quit,4)==0) break;
		}
		char** tokens= get_tokens(line,&size);
		if(strcmp(tokens[0],"cd")==0)my_cd(tokens,size);
		else if(strcmp(tokens[0],"wc")==0)my_wc(tokens);
		// else if(strcmp(tokens[0],"pwd")==0)execute("/bin/",tokens);
		else if(strcmp(tokens[0],"echo")==0 || strcmp(tokens[0],"ls")==0)execute("/bin/",tokens);
		else printf("Command not found\n");
	}


	return 0;
}
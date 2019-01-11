#include "../shell.h"

//it will execute process taking input from "input" and output from "output" fd's respectively
// flag means whether to process execvp or builtin command

int execute_process(char * array[],int input, int output){
	pid_t pid = fork();
	if(pid < 0){
		fprintf(stderr,"Child process could not be created\n");
		return 0;
	}
	else if(pid==0)
	{
		if(input!=0) close(0);
		if(output!=1) close(1);
		dup2(input,0); dup2(output, 1); // set the inp/op sources accordingly
		if (execvp(array[0], array)<0)
		{
			fprintf(stderr, "%s:command could not be executed\n",array[0]);
			exit(0);
		}	
	}
	else{
		wait(NULL);
		return 1;
	}
}
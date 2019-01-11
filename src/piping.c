#include "../shell.h"

int pipehandler(char **args, int num)
{
	//args: space separated list of arguments of a command
	int pipestr[2], i = -1, j=0, k=0, start = 0;
	int num_of_pipes;
	char * array[100];
	while (args[++i])
	{
		if (strcmp(args[i], "|") == 0)
		{
			if (j == 0) start = i;
			++j;
		}
	}
	int cmds = j + 1;
	if (start == 0)
	{
		fprintf(stderr, "mysh: syntax error in command entered\n");
		return 0;
	}
	else
	{
		int fd_in=0,commands_done=0;
		for (i=0;i<num;++i){
			j=0;
			while(args[i] && strcmp(args[i],"|")!=0 ){
				array[j++]=args[i++];
			}
			array[j]=0;
			pipe(pipestr);
			int ret=0,k=-1;

			if(commands_done==0){
				ret = redirect(array,j,fd_in,pipestr[1]);
			}
			else if(commands_done==cmds-1){
				ret = redirect(array,j,fd_in,1);
			}
			else{
				ret = execute_process(array,fd_in,pipestr[1]);
			}
			if(ret==0) return 0;
			close(pipestr[1]);
      		fd_in = pipestr[0];
			++commands_done;
		}
	}
	return 1;
}
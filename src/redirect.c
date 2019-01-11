#include "../shell.h"

int redirect(char *array[], int len, int input, int output)
{
    //one complete command without any pipes
    array[len] = NULL;
    char output_file1[PATH_SIZE], output_file2[PATH_SIZE], input_file[PATH_SIZE];
    int indirec = 0, outdirec = 0, outappend = 0; //flags for checking
    int i = -1;

    while (array[++i])
    {
        if (strcmp(array[i], "<") == 0)
        {
            if(!array[i+1]) {
                fprintf(stderr, "mysh: syntax error: Invalid usage of pipes or redirection\n");
                return 1;
            }
            array[i] = NULL;
            // stdin should become array[i+1]
            strcpy(input_file, array[i+1]);
            indirec = 1;
        }
        else if (strcmp(array[i], ">") == 0)
        {
            if(!array[i+1]) {
                fprintf(stderr, "mysh: syntax error: Invalid usage of pipes or redirection\n");
                return 1;
            }
            array[i] = NULL;
            // stdout should become array[i+1]
            strcpy(output_file1, array[i + 1]);
            outdirec = 1;
        }
        else if (strcmp(array[i], ">>") == 0)
        {
            if(!array[i+1]) {
                fprintf(stderr, "mysh: syntax error: Invalid usage of pipes or redirection\n");
                return 1;
            }
            array[i] = NULL;
            //stdout should become array[i+1] and append to file
            strcpy(output_file2, array[i+1]);
            outappend = 1;
        }
        // array[i] = args[i];
    }

    int fd_inp=input, fd_op=output;
    if (indirec){
        fd_inp = open(input_file, O_RDONLY, 0);
        if (fd_inp < 0){
            perror("Could not open input file");
            return 1;
        }
    }
    if (outdirec)
    {
        fd_op = open(output_file1, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_op < 0){
            perror("Could not open output file");
            return 1;
        }
    }
    else if (outappend)
    {
        fd_op = open(output_file2, O_APPEND | O_WRONLY | O_CREAT, 0644);
        if (fd_op < 0){
            perror("Could not open output file");
            return 1;
        }
    }

    int ret = 0;
    // it's not a builtin process so execute via execvp
    ret = execute_process(array,fd_inp,fd_op);
    if(ret==0) return 0;
    return 1;
}
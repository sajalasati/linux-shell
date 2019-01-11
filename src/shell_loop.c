#include "../shell.h"

void sigzzhandler(int signum)
{
    return;
}

void shell_loop()
{
    char *line;
    char **cmd; //commands array separted by ; in line
    int status = 1;
    while (status)
    {
        signal(SIGINT, sighandler);
        signal(SIGTSTP, sigzzhandler);
        child_exit_handler();
        print_prompt();
        char *line = read_line();
        cmd = get_args(line, 0);
        status = execute(cmd);
        free(line);
        free(cmd);
    }
}
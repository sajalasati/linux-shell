#include "../shell.h"

void child_exit_handler()
{
    char *col1 = YELLOW;
    char *col2 = NC;
    char *col3 = GREEN;
    char *col4 = RED;
    int status;
    pid_t wpid = waitpid(-1, &status, WNOHANG);
    if (wpid > 0 && WIFEXITED(status) > 0)
    {
        fprintf(stderr, "\nProcess %s%s%s with pid: %s%d%s exited %snormally%s\n", col1, bg_processes[wpid], col2, col1, wpid, col2, col3, col2);
        proc_status[wpid] = -1;
        bg_processes[wpid] = '\0';
    }
    else if (wpid > 0 && WIFSIGNALED(status) == 1)
    {
        fprintf(stderr, "\nProcess %s%s%s with pid: %s%d%s exited %sabnormally%s due to signal %s%d%s\n", col1, bg_processes[wpid], col2, col1, wpid, col2, col4, col2, col1, WTERMSIG(status), col2);
        proc_status[wpid] = -1;
        bg_processes[wpid] = '\0';
    }
    return;
}

void sighandler(int signum)
{
    char *col1 = YELLOW;
    char *col2 = NC;
    if (foreground > 0)
    {
        kill(foreground, SIGINT);
        foreground = -1;
    }
    printf("\n%sKeyboard interrupt, PRESS ENTER to continue...%s\n", col1, col2);
    fflush(stdout);
}

void clock_handler(int signum)
{
    sigc_flag = 1;
    run_clock = 0;
    printf("\nExiting Clock\n");
}
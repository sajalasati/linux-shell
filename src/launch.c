#include "../shell.h"
int signal_z;
char *stopcmd;

void sigzhandler(int signum)
{
    proc_status[fg_pid] = 0; // 0 means stopped
    printf("\nStopped : [%d] \n", fg_pid);
    kill(fg_pid, SIGTSTP);
    signal_z = 1;
    fflush(stdout);
    return;
}

int launch(char **args)
{
    signal_z = 0;
    //args is arguments of space spearated single command
    int background = 0, redirection = 0, piping = 0;
    int i, j;
    char *array[200];
    for (i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "quit") == 0 || strcmp(args[i], "exit") == 0)
            exit(EXIT_SUCCESS);
        for (j = 0; args[i][j] != '\0'; j++)
        {
            if (args[i][j] == '>' || args[i][j] == '<')
                redirection = 1;
            if (args[i][j] == '|')
                piping = 1;
        }
    }
    i = -1;
    while (args[++i])
    {
        if (strcmp(args[i], "&") == 0)
        {
            args[i] = 0;
            background = 1;
            break;
        }
        array[i] = args[i];
    }

    if (piping)
        return pipehandler(args, i);
    if (redirection)
        return redirect(array, i, 0, 1);

    //normal commands without pipes/redirection
    int ret = execute_builtins(args, 0, 1);
    if (ret == 1)
        return 1;
    pid_t cpid, w;
    int status, flag = 0;
    signal(SIGTSTP, sigzhandler);
    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        //child process
        if (background == 1)
        {
            if (strcmp(args[0], "vim") == 0 || strcmp(args[0], "vi") == 0)
            {
                exit(EXIT_SUCCESS);
            }
            else
            {
                setpgid(0, 0);
            }
        }
        if (execvp(args[0], args) == -1)
        {
            perror("mysh");
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        if (background == 1)
        {
            if (strcmp(args[0], "vim") != 0 && strcmp(args[0], "vi") != 0)
            {
                proc_status[cpid] = 2; // 1 means running
                printf("%d running in background\n", cpid);
                bg_order[bg_order_len++] = cpid;
                bg_processes[cpid] = malloc(1024);
                strcpy(bg_processes[cpid], args[0]);
                return 1;
            }
            else
            {
                proc_status[cpid] = 1; // 1 means running
                printf("\nStopped : [%d] \n", cpid);
                bg_order[bg_order_len++] = cpid;
                bg_processes[cpid] = malloc(1024);
                strcpy(bg_processes[cpid], args[0]);
                return 1;
            }
        }
        do
        {
            fg_pid = cpid;
            stopcmd = malloc(1024);
            strcpy(stopcmd, args[0]);
            if (signal_z == 0)
            {
                w = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
                if (w == -1)
                {
                    perror("waitpid");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                bg_order[bg_order_len++] = fg_pid;
                bg_processes[fg_pid] = malloc(1024);
                strcpy(bg_processes[fg_pid], args[0]);
                break;
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        free(stopcmd);
        return 1;
    }
}

int builtin_jobs(char **argv)
{
    int i, j = 1;
    for (i = 0; i < 32768; i++)
    {
        //avoid the non existing processes
        if (bg_processes[bg_order[i]] && proc_status[bg_order[i]] != -1)
        {
            int pid = bg_order[i];
            char addr[1000]; //path to stat file
            sprintf(addr, "/proc/%d/stat", pid);
            FILE *f;
            if (!(f = fopen(addr, "r")))
            {
                fprintf(stderr, "pinfo Error : No such process found\n");
                return 1;
            }
            char status, name[200];
            fscanf(f, "%d %s %c", &pid, name, &status);
            printf("[%d]", j++);
            if (status == 'R')
                printf("\tRunning");
            if (status == 'S')
                printf("\tRunning");
            if (status == 'T')
                printf("\tStopped");
            if (status == 'Z')
                printf("\tZombie");
            // if (proc_status[bg_order[i]] == 1)
            //     printf("\tRunning");
            // else
            //     printf("\tStopped");

            printf("\t%s [%d]\n", bg_processes[bg_order[i]], bg_order[i]);
        }
    }
    return 1;
}

int builtin_kjob(char **args)
{
    int pid, found = 0;
    if (!args[1] || !args[2])
    {
        perror("Insuffecient arguments");
    }
    else
    {
        int j = 0, job_id = atoi(args[1]), signal_id = atoi(args[2]);
        for (int i = 0; i < 32768; i++)
        {
            if (bg_processes[bg_order[i]] && proc_status[bg_order[i]] != -1)
            {
                j++;
                if (j == job_id)
                {
                    pid = bg_order[i];
                    found = 1;
                    break;
                }
            }
        }
        if (found == 0)
        {
            perror("process does not exist with given job id");
        }
        else
        {
            if (kill(pid, atoi(args[2])) == -1)
            {
                perror("signal could not be killed");
            }
            if (signal_id == SIGKILL)
            {
                proc_status[pid] = -1; //only manipulate the proc_status not bg_processes
            }
        }
    }
    return 1;
}

int builtin_overkill(char **args)
{
    for (int i = 0; i < 32768; i++)
    {
        if (bg_processes[bg_order[i]] && proc_status[bg_order[i]] != -1)
        {
            proc_status[bg_order[i]] = -1;
            kill(bg_order[i], 9);
        }
    }
    return 1;
}

int builtin_fg(char **args)
{
    int pid, done = 0;
    signal_z = 0;
    signal(SIGTSTP, sigzhandler);
    if (!args[1] || args[2])
    {
        fprintf(stderr, "Incorrect number of arguments\n");
        fprintf(stderr, "correct usage: fg <job_no>\n");
    }
    else
    {
        int j = 0, job_id = atoi(args[1]);
        for (int i = 0; i < 32768; i++)
        {
            if (bg_processes[bg_order[i]] && proc_status[bg_order[i]] != -1)
            {
                j++;
                if (j == job_id)
                {
                    //to be run in foreground so better remove it from jobs array
                    pid = bg_order[i];
                    foreground = pid;
                    proc_status[pid] = 1;
                    // bg_order_len--;
                    fg_pid = pid;
                    stopcmd = malloc(1024);
                    strcpy(stopcmd, bg_processes[pid]);
                    int status;
                    kill(pid, SIGCONT);
                    waitpid(pid, &status, WUNTRACED);
                    if (WIFEXITED(status) || WIFSIGNALED(status))
                        proc_status[pid] = -1;
                    done = 1;
                    break;
                }
            }
        }
        if (done == 0)
            perror("process does not exist with given job id");
    }
    return 1;
}

int builtin_bg(char **args)
{
    int pid, done = 0;
    if (!args[1])
    {
        perror("Insuffecient arguments");
    }
    else
    {
        int j = 0, job_id = atoi(args[1]);
        for (int i = 0; i < 32768; i++)
        {
            if (bg_processes[bg_order[i]] && proc_status[bg_order[i]] != -1)
            {
                j++;
                if (j == job_id)
                {
                    pid = bg_order[i];
                    int status;
                    kill(pid, SIGCONT);
                    proc_status[pid] = 1; //running
                    done = 1;
                    break;
                }
            }
        }
        if (done == 0)
            perror("process does not exist with given job id");
    }
    return 1;
}
#include "../shell.h"

char **get_args(char *line, int decide)
{
    char *DELIM;
    if (decide == 0)
        DELIM = COMM_DELIM;
    else
        DELIM = TOK_DELIM;
    size_t buf_size = RL_BUFSIZE;
    int cmd_buf_size = buf_size;
    char **cmd = malloc(buf_size * sizeof(char *));
    //means cmd[i] points to ith character array
    if (!cmd)
    {
        fprintf(stderr, "mysh: Allocaiton Error \n");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    char *command = strtok(line, DELIM);
    // printf("Following is the list of commands found:\n");
    while (1)
    {
        if (command == NULL)
            break;
        if (i >= buf_size)
        {
            buf_size += RL_BUFSIZE;
            cmd_buf_size = buf_size;
            cmd = realloc(cmd, buf_size);
        }
        if (!cmd)
        {
            fprintf(stderr, "mysh: Allocation Error \n");
            exit(EXIT_FAILURE);
        }
        cmd[i++] = command;
        // printf("%s\n", command); //maybe print or not print this
        command = strtok(NULL, DELIM);
    }
    cmd[i] = NULL;
    return cmd;
}

char *read_line()
{
    size_t buf_size = RL_BUFSIZE;
    char *buf = (char *)malloc(buf_size);
    if (!buf)
    {
        fprintf(stderr, "Allocaiton Error \n");
        exit(EXIT_FAILURE);
    }
    int i = 0, ch;
    while (1)
    {
        ch = getchar();
        if (ch == EOF)
            exit(0);
        else if (ch == '\n')
        {
            buf[i++] = '\0';
            return buf;
        }
        else
            buf[i++] = ch;

        if (i >= buf_size)
        {
            buf_size += RL_BUFSIZE;
            buf = realloc(buf, buf_size);
            if (!buf)
            {
                fprintf(stderr, "Allocaiton Error \n");
                exit(EXIT_FAILURE);
            }
        }
    }
}
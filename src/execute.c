#include "../shell.h"

int execute(char **cmd)
{
    int cmdLen = -1, ret = 1;
    char **args; //arguments of command
    while (cmd[++cmdLen] != NULL)
    {
        args = get_args(cmd[cmdLen], 1);
        int flag = 0, i = -1;
        if (args[0] == NULL)
        {
            ret = 0;
            flag++;
        }
        if (flag == 0)
            ret = launch(args);
        free(args);
        if (ret == 0)
            return 0;
    }
    return 1;
}
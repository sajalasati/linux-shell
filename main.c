#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "shell.h"

int main(int argc, char **argv)
{
    run_clock = 1;
    bg_order_len = 0;
    foreground = -1;
    for (int i = 0; i < 32768; ++i)
        proc_status[i] = -1;
    home = getenv("PWD"); //home needs to the first opened directory
    shell_loop();
    return EXIT_SUCCESS;
}
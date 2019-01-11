#include "../shell.h"

void print_prompt()
{
    char *user;
    user = getenv("USER");
    struct utsname sys_details;
    if (uname(&sys_details) != 0)
    {
        perror("uname");
        exit(EXIT_FAILURE);
    }
    char *cur_dir = (char *)malloc(1024);
    if (!cur_dir)
    {
        fprintf(stderr, "Allocaiton Error \n");
        exit(EXIT_FAILURE);
    }
    getcwd(cur_dir, (size_t)1024);
    char *rel_path = get_relative_path(cur_dir);
    char *col1 = GREEN;
    char *col2 = NC;
    printf("%s%s@%s%s:%s> ", col1, user, sys_details.nodename, col2, rel_path);
    free(cur_dir);
}
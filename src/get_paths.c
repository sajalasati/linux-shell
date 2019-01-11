#include "../shell.h"

char *get_relative_path(char *cur_dir)
{
    int cur_size = strlen(cur_dir), home_size = strlen(home);
    char *arr = "~";
    if (cur_size == home_size && strcmp(cur_dir, home) == 0)
        return arr;
    else if (cur_size < home_size)
    {
        return cur_dir;
    }
    else
    {
        char *temp = (char *)malloc((size_t)home_size);
        for (int i = 0; i < home_size; ++i)
        {
            temp[i] = cur_dir[i];
        }
        temp[home_size] = 0;
        if (strcmp(temp, home) != 0)
            return cur_dir;
        free(temp);
        char *rel_path = (char *)malloc((size_t)PATH_SIZE);
        if (!rel_path)
        {
            fprintf(stderr, "Allocaiton Error \n");
            exit(EXIT_FAILURE);
        }
        rel_path[0] = '~';
        int count = 1;
        for (int i = home_size; i < cur_size; ++i)
        {
            rel_path[count++] = cur_dir[i];
        }
        rel_path[count] = '\0';

        return rel_path;
    }
}

char *get_absolute_path(char *given_path)
{
    char *temp = malloc((size_t)1024);
    int i = 0, start = 0;
    if (given_path[0] == '~')
    {
        while (home[i] != '\0')
        {
            temp[i] = home[i];
            i++;
        }
        start = 1;
    }
    int j = i;
    while (given_path[start] != '\0')
    {
        temp[j++] = given_path[start++];
    }
    temp[j] = '\0';
    return temp;
}
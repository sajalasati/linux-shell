#include "../shell.h"

int builtin_ls(char **args)
{
    int n, begin = 1;
    bool hidden = false, long_format = false;
    int i = 0, allowed = 0;
    while (args[++i])
    {
        if (args[i][0] == '-')
        {
            if (args[1] && strcmp(args[i], "-l") == 0)
            {
                long_format = true;
            }
            else if (args[1] && strcmp(args[i], "-a") == 0)
            {
                hidden = true;
            }
            else if (args[1] && strcmp(args[i], "-la") == 0)
            {
                long_format = true;
                hidden = true;
            }
            else if (args[1] && strcmp(args[i], "-al") == 0)
            {
                long_format = true;
                hidden = true;
            }
            else
            {
                fprintf(stderr, "Unexpected arguments provided to \"ls\"\n");
                return 1;
            }
        }
    }
    i = 1;
    int exit_flag = 0, dir_till_now = 0;
    while (1)
    {
        if ((!args[i] && dir_till_now == 0) || (args[i] && args[i][0] != '-'))
        {
            if (args[i] && args[i][0] != '-')
                dir_till_now++;
            struct passwd *user;
            struct group *group;
            struct tm *tm;
            char *temp;
            DIR *dir_p;
            struct dirent *dir_element;
            struct stat file_info;
            char this_dir[] = ".";
            char date_str[1024];
            if (!args[i])
            {
                temp = this_dir;
                exit_flag++;
            }
            else
                temp = get_absolute_path(args[i]);
            //if it isn't a directory
            if (!(dir_p = opendir(temp)))
            {
                perror("opendir");
                return 1;
            }
            printf("\n");
            while (dir_element = readdir(dir_p))
            {
                char *full_path = malloc(1024 * sizeof(char));
                sprintf(full_path, "%s/%s", temp, dir_element->d_name);
                stat(full_path, &file_info);
                char *name = dir_element->d_name;
                if (!hidden && name[0] == '.')
                    continue;
                else
                {
                    if (long_format)
                    {
                        printf((S_ISDIR(file_info.st_mode)) ? "d" : "-");
                        printf((file_info.st_mode & S_IRUSR) ? "r" : "-");
                        printf((file_info.st_mode & S_IWUSR) ? "w" : "-");
                        printf((file_info.st_mode & S_IXUSR) ? "x" : "-");
                        printf((file_info.st_mode & S_IRGRP) ? "r" : "-");
                        printf((file_info.st_mode & S_IWGRP) ? "w" : "-");
                        printf((file_info.st_mode & S_IXGRP) ? "x" : "-");
                        printf((file_info.st_mode & S_IROTH) ? "r" : "-");
                        printf((file_info.st_mode & S_IWOTH) ? "w" : "-");
                        printf((file_info.st_mode & S_IXOTH) ? "x" : "-");
                        printf(" %4lu ", file_info.st_nlink);
                        if ((user = getpwuid(file_info.st_uid)) != NULL)
                        {
                            printf(" %s", user->pw_name);
                        }
                        else
                        {
                            printf(" %d", file_info.st_uid);
                        }
                        if ((group = getgrgid(file_info.st_gid)) != NULL)
                        {
                            printf(" %s", group->gr_name);
                        }
                        else
                        {
                            printf(" %d", file_info.st_gid);
                        }
                        printf(" %9jd", (__intmax_t)file_info.st_size);
                        tm = localtime(&file_info.st_mtime);
                        strftime(date_str, sizeof(date_str), "%b %d %H:%M", tm);
                        printf(" %s", date_str);
                    }
                    char *col1 = BLUE;
                    char *col2 = NC;
                    if (S_ISDIR(file_info.st_mode))
                    {
                        printf("%s", col1);
                    }
                    printf(" %s%s\n", dir_element->d_name, col2);
                }
                free(full_path);
            }
        }
        else if (!args[i] && dir_till_now != 0)
            exit_flag++;
        ++i;
        printf("\n");
        if (exit_flag != 0)
        {
            break;
        }
    }
    return 1;
}
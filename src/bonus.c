#include "../shell.h"

int builtin_remindme(char **args)
{
    if (!args[1] || !args[2])
    {
        fprintf(stderr, "mysh: unexpected number of arguments to \"remindme\"\n");
        return 1;
    }
    int interval = atoi(args[1]);
    pid_t cpid = fork();

    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        /* Code executed by child */
        sleep(interval);
        int i = 2;
        char *col1 = YELLOW;
        char *col2 = NC;
        printf("\n\nRemindMe Says: %s", col1);
        while (args[i])
        {
            printf("%s ", args[i++]);
        }
        printf("%s\n\n", col2);
        exit(EXIT_SUCCESS);
    }
    else
    {
        return 1;
    }
    return 1;
}
int builtin_clock(char **args)
{
    signal(SIGINT, clock_handler);
    char date_addr[1000], time_addr[1000];
    char date_arr[1000], time_arr[1000];
    sprintf(date_addr, "/sys/devices/pnp0/00:03/rtc/rtc0/date");
    sprintf(time_addr, "/sys/devices/pnp0/00:03/rtc/rtc0/time");
    if ((!args[1] || !args[2]) || (args[1] && args[2] && args[3] && !args[4]))
    {
        fprintf(stderr, "mysh: unexpected number of arguments to \"clock\"\n");
        fprintf(stderr, "mysh: type \"clock\" --help to know the correct usage\n");
        return 1;
    }
    else if (args[1] && (strcmp(args[1], "--help") == 0))
    {
        printf("Usage: clock -t <interval> -n <time for clock to run>\n");
        printf("   OR  clock -t <interval>\n");
        return 1;
    }
    if (!args[3] || !args[4])
    {
        int interval = atoi(args[2]);
        FILE *f1, *f2;
        while (run_clock)
        {
            if (!(f1 = fopen(date_addr, "r")))
            {
                perror("fopen");
                return 1;
            }
            if (!(f2 = fopen(time_addr, "r")))
            {
                perror("fopen");
                return 1;
            }
            char *col1 = YELLOW;
            char *col2 = NC;
            fscanf(f1, "%s", date_arr);
            fscanf(f2, "%s", time_arr);
            fprintf(stdout, "%s%s%s, %s%s%s\n", col1, date_arr, col2, col1, time_arr, col2);
            fclose(f1);
            fclose(f2);
            sleep(interval);
        }
        run_clock = 1;
    }
    else
    {
        int interval = atoi(args[2]);
        int len = atoi(args[4]);
        FILE *f1, *f2;
        signal(SIGINT, clock_handler);
        printf("Running the clock for %d seconds\n", len);
        while (len > 0 && run_clock)
        {
            if (!(f1 = fopen(date_addr, "r")))
            {
                perror("fopen");
                return 1;
            }
            if (!(f2 = fopen(time_addr, "r")))
            {
                perror("fopen");
                return 1;
            }
            char *col1 = YELLOW;
            char *col2 = NC;
            fscanf(f1, "%s", date_arr);
            fscanf(f2, "%s", time_arr);
            fprintf(stdout, "%s%s%s, %s%s%s\n", col1, date_arr, col2, col1, time_arr, col2);
            fclose(f1);
            fclose(f2);
            sleep(interval);
            len -= interval;
        }
        run_clock = 1;
        return 1;
    }
    return 1;
}
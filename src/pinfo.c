#include "../shell.h"

int builtin_pinfo(char **args)
{
    char addr[1000]; //path to stat file
    char temp[1000];
    if (args[1])
    {
        sprintf(temp, "/proc/%s/", args[1]);
        sprintf(addr, "/proc/%s/stat", args[1]);
    }
    else
    {
        sprintf(temp, "/proc/%d/", getpid());
        sprintf(addr, "/proc/%d/stat", getpid());
    }
    //pid and process status
    FILE *f;
    if (!(f = fopen(addr, "r")))
    {
        fprintf(stderr, "pinfo Error : No such process found\n");
        return 1;
    }
    int pid;
    char status, name[200];
    char *col1 = YELLOW;
    char *col2 = NC;
    char *col3 = RED;
    fscanf(f, "%d %s %c", &pid, name, &status);
    printf("pid -- %s%d%s\n", col1, pid, col2);
    printf("Process Status -- %s%c%s\n", col3, status, col2);
    fclose(f);

    //virtual memory
    long long int v_mem;
    char mem_addr[1000]; //path to statm file
    strcpy(mem_addr, temp);
    strcat(mem_addr, "statm");
    if (!(f = fopen(mem_addr, "r")))
    {
        fprintf(stderr, "pinfo Error : No such process found\n");
        return 1;
    }
    fscanf(f, "%Ld", &v_mem);
    printf("%Ld {Virtual Memory}\n", v_mem);
    fclose(f);

    //path to executable
    char exec_path[1000], exec_addr[1000] = {0}; //exec_addr is path to exe file
    strcpy(exec_path, temp);
    strcat(exec_path, "exe");
    int len = readlink(exec_path, exec_addr, sizeof(exec_addr));
    if (len >= 0)
    {
        exec_addr[len] = '\n';
        printf("Executable Path -- %s%s%s", col1, exec_addr, col2);
    }
    else
        perror("readlink");
    return 1;
}
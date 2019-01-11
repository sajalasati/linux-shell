#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <fcntl.h>

char *home;
char *rel_path;
#define TOK_DELIM " \t\n\a\r"
#define RL_BUFSIZE 1024
#define PATH_SIZE 256
#define COMM_BUFFER 64
#define COMM_DELIM ";"

#define RED "\033[31;1m"
#define GREEN "\033[32;1m"
#define YELLOW "\033[33;1m"
#define BLUE "\033[34;1m"
#define NC "\033[0m"

void child_exit_handler();
int builtin_cd(char **args);
int builtin_exit(char **args);
int builtin_quit(char **args);
int builtin_pwd(char **args);
int builtin_echo(char **args);
int builtin_ls(char **args);
int builtin_pinfo(char **args);
int builtin_clock(char **args);
int builtin_remindme(char **args);
char *get_relative_path(char *cur_dir);
char *get_absolute_path(char *given_path);
int execute(char **cmd);
int launch(char **args);
char **get_args(char *line, int decide);
char *read_line();
void print_prompt();
void shell_loop();
void sighandler(int signum);
void clock_handler(int signum);
int redirect(char **args, int num, int input, int output);
int pipehandler(char **args, int num);
int execute_process(char *array[], int input, int output);
int execute_builtins(char **args, int input, int output);
int builtin_unsetenv(char **args);
int builtin_setenv(char **args);
int builtin_jobs(char **argv);
int builtin_kjob(char **args);
int builtin_overkill(char **args);
int builtin_fg(char **args);
int builtin_bg(char **args);
int fg_pid;

int run_clock, sigz_flag, sigc_flag;
int bg_order_len, bg_order[32768], foreground;
int proc_status[32768]; // -1 means not exist, 1 means running, 0 means stopped
char *bg_processes[32768];

#endif
# Mysh

A simple shell implemented in C. Written as a part of Operating Systems coursework.

It should work in the similar manner as popular shells like bash, zsh etc. To run a command, type it into the shell and press Enter. All commands are terminated with newline character.

### Features:

- Modular Code
  - One header file
  - Each functionality (command) is defined in different file.
- Color added for specific purposes.
- Proper error handling for each command.
- Proper handling of Ctrl+Z and Ctrl+C - similar to what happens in a normal terminal.
- Input/output redirection functionality.
- Command can be redirected using pipes as well as combination of pipes and I/O redirections.
- All basic features implemented.
  - Semicolon separated commands accepted
  - Builtin commmands like cd, echo, pwd.
  - Background and foreground processes.
  - Notification upon completion of finished background processes.
  - ls command implemented separately as required in the assignment with arguments -[al]
  - Exit and Quit commands for quitting the shell
- Non Standard "user-defined" commands:
  - "pinfo" : to display specific process related information.
  - "remindme" : a simple Personal Reminder.
  - "clock" : a command which displays dynamic date and time in your shell. There are two ways to run the clock, type clock --help to see.
  - setenv var [value] : create an environment variable if it does not exist.
  - unsetenv var : destroy the environment variable "var".
  - jobs : prints a list of all currently running jobs along with their pid, in particular, background jobs, in order of their creation along with their state.
  - kjob <jobNumber> <signalNumber> : takes the job id of a running job and sends a signal value to that process.
  - fg <jobNumber> : brings a running or a stopped background job with given job number to foreground.
  - bg <jobNumber> : changes a stopped background job to a running background job.
  - overkill : kills all background process at once.

### Building and Running

    cd mysh_shell
    make
    ./mysh

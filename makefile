mysh: \
			main.o \
			shell_loop.o \
			prompt.o \
			pinfo.o \
			parse.o \
			ls.o \
			launch.o \
			handlers.o \
			get_paths.o \
			execute.o \
			builtins.o \
			piping.o \
			process.o \
			bonus.o \
			redirect.o	
	$(CC) -g -o mysh $^

clean:
	@rm -f *.o shell

main.o: shell.h main.c
	$(CC) -g -c main.c

shell_loop.o: shell.h src/shell_loop.c
	$(CC) -g -c src/shell_loop.c

prompt.o: shell.h src/prompt.c
	$(CC) -g -c src/prompt.c

pinfo.o: shell.h src/pinfo.c
	$(CC) -g -c src/pinfo.c

parse.o: shell.h src/parse.c
	$(CC) -g -c src/parse.c

ls.o: shell.h src/ls.c
	$(CC) -g -c src/ls.c

launch.o: shell.h src/launch.c
	$(CC) -g -c src/launch.c

handlers.o: shell.h src/handlers.c
	$(CC) -g -c src/handlers.c

get_paths.o: shell.h src/get_paths.c
	$(CC) -g -c src/get_paths.c

execute.o: shell.h src/execute.c
	$(CC) -g -c src/execute.c

redirect.o: shell.h src/redirect.c
	$(CC) -g -c src/redirect.c

piping.o: shell.h src/piping.c
	$(CC) -g -c src/piping.c

process.o: shell.h src/process.c
	$(CC) -g -c src/process.c

builtins.o: shell.h src/builtins.c
	$(CC) -g -c src/builtins.c

bonus.o: shell.h src/bonus.c
	$(CC) -g -c src/bonus.c
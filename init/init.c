#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <poll.h>

#include <dirent.h>


//Needs NULL as last arg!!
int run_start(char* _prog, char* _args[])
{
	pid_t pid = fork();

	if( pid < 0 )
		return 1;
	else if (pid == 0)
	{
		//wait(0);
		return 0;
	}

	setsid();
	if(fork())
		exit(0);

	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	close(STDERR_FILENO);

	execv(_prog, _args);

	exit(1);
}

//Need NULL as last arg!!
int run_stdio(char* _prog, const char* _args[])
{	
	int fork_pipe_out[2];
	int fork_pipe_in [2];
	pid_t pid;

	if(pipe(fork_pipe_out) == -1)
		return 1;

	if(pipe(fork_pipe_in) == -1)
		return 1;

	if((pid = fork()) == -1)
		return 2;

	if (pid == 0)
	{
		//replace STDIO
		dup2(fork_pipe_out[1], STDOUT_FILENO);
		dup2(fork_pipe_in [0], STDIN_FILENO );
		close (fork_pipe_out[0]);
		close (fork_pipe_in [1]);

		execv(_prog, _args);

		//Error handling
		int err = errno;
		printf("Fuckup %i: %s\n", err, strerror(err));

		exit(1);
	}
	else
	{
		close(fork_pipe_out[1]);
		close(fork_pipe_in [0]);
		
		int ret;
		struct pollfd fds[2];
		fds[0].fd = STDIN_FILENO;
		fds[0].events = POLLIN;

		fds[1].fd = fork_pipe_out[0];
		fds[1].events = POLLIN;

		while(1)//Check for data while pipe is open
		{
			int timeout = 1; //Timeout msecs
			ret = poll(fds, 2, timeout);

			if(ret < 0)
				break;
			else if (ret == 0)
				continue;
			

			int nbytes;
			char buffer[256];

			if(fds[0].revents & POLLIN) //STDIN_FILENO
			{
				nbytes = read(STDIN_FILENO, buffer, sizeof(buffer));
				write(fork_pipe_in[1], buffer, nbytes);
				//write(fork_pipe_in[1], "\n", 1);
				
			}
			if(fds[1].revents & POLLIN) //PIPE
			{
				nbytes = read(fork_pipe_out[0], buffer, sizeof(buffer));
				write(STDOUT_FILENO, buffer, nbytes);
				//write(STDOUT_FILENO, "\n", 1);
			}
		}
		wait(0);	
	}
}

int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
	printf("MegumInit explosion!\n");

	//char* arg[] = {"/bin/bash" ,(char*)0};

	//run_start(arg[0], arg);
	
	char* arg2[] = {"/bin/toybox", "echo", "/test.asdf", NULL};

	run_start(arg2[0], arg2);

	sleep(2);
	
	printf("%i\n", exists("/test.asdf"));
	printf("\nEND\n");

	sleep(99999);

	return 0;
}

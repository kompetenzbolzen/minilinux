#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <poll.h>

int run_start(char* _prog)
{
	pid_t pid = fork();

	if( pid < 0 )
		return 1;
	else if (pid == 0)
		return 0;

	execl(_prog, _prog, NULL);
}

int run_stdio(char* _prog)
{
	
	int fork_pipe_out[2];
	int fork_pipe_in [2];
	pid_t pid = 0;

	if(pipe(fork_pipe_out) == -1)
		return 1;

	if(pipe(fork_pipe_in) == -1)
		return 1;

	if((pid = fork()) == -1)
		return 2;

	if (pid == 0)
	{
		dup2(fork_pipe_out[1], STDOUT_FILENO);
		dup2(fork_pipe_in [0], STDIN_FILENO );
		close (fork_pipe_out[0]);
		close (fork_pipe_in [1]);
		
		execl(_prog, _prog, NULL);

		return 3;
	}
	else
	{
		printf("asdf\n");
		close(fork_pipe_out[1]);
		close(fork_pipe_in [0]);
		
		int ret;
		struct pollfd fds[2];
		fds[0].fd = STDIN_FILENO;
		fds[0].events = POLLIN;

		fds[1].fd = fork_pipe_out[0];
		fds[1].events = POLLIN;

		while(1)
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
				printf("a\n");
				nbytes = read(STDIN_FILENO, buffer, sizeof(buffer));
				write(fork_pipe_in[1], buffer, nbytes);
				write(fork_pipe_in[1], "\n", 1);
				
			}
			if(fds[1].revents & POLLIN) //PIPE
			{
				printf("b\”");
				nbytes = read(fork_pipe_out[0], buffer, sizeof(buffer));
				write(STDOUT_FILENO, buffer, nbytes);
				write(STDOUT_FILENO, "\n", 1);
			}
		}
		wait(0);	
	}
}

int main(int argc, char* argv[])
{
	printf("MegumInit explosion!\n");
	run_stdio("yes");

	return 0;
}

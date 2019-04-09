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
	else if (pid > 0)
		return 0;

	
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
	pid_t pid;
	if((pid = fork()) == -1)
		return 2;

	if (pid == 0)
	{
		execv(_prog, _args);

		//Error handling
		int err = errno;
		printf("Fuckup %i: %s\n", err, strerror(err));

		exit(1);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		return 0;
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
	printf("MegumInit\n");
	printf("(∩ಠ益[+])⊃━☆ﾟExplosion!\n\n");
	
	char* arg2[] = {"/bin/toybox", "cat", "meg.txt", NULL};
	run_stdio(arg2[0], arg2);

	return 0;
}

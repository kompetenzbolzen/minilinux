#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	printf("I'm a kernel!\r\n");

	while(1)
	{
		printf("Still running!\r\n");
		sleep(1);
	}
}

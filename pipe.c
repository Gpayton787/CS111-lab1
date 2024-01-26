#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	// TODO: it's all yours
	printf("%i\n", argc);
	for(int i = 0; i < argc; i++){
		printf(argv[i]);
	}
	
	return 0;
}

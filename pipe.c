#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	// First print args
	// printf("argc: %i\n", argc);
	// for(int i = 0; i < argc; i++){
	// 	printf("arg %i: %s\n", i, argv[i]);
	// }

	int fds[2];
	pipe(fds); //Create a pipe

	int return_code = fork();
	if(return_code == 0){
		//Child
		printf("Child Process: \n");

	}
	else if(return_code > 0){
		printf("Parent Process: \n");

	}
	else{
		printf("Child process creation error\n");
	}
	
	return 0;
}

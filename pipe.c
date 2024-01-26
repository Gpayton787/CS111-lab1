#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	// First print args
	printf("argc: %i\n", argc);
	for(int i = 0; i < argc; i++){
		printf("arg %i: %s\n", argv[i]);
	}
	//Test fork
	int return_code = fork();
	if(return_code == 0){
		//Child
		printf("Child Process: \n");
		execlp("ls", "ls", "NULL");
		exit(0);
	}
	else if(return_code > 0){
		printf("Parent Process: \n");
		printf("Wait for the child to finish\n");
		int pid = return_code;
		int status = 0;
		waitpid(pid, &status, 0);
		printf("Child process done, exited with code: %d\n", WEXITSTATUS(status));
	}
	else{
		printf("Child process creation error\n");
	}
	
	return 0;
}

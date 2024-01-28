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
	//Try piping now

	int fds[2];
	pipe(fds); //Create a pipe

	int return_code = fork();
	if(return_code == 0){
		//Child
		printf("Child Process: \n");
		dup2(fds[1], STDOUT_FILENO);
		execlp("ls", "ls", NULL);
		printf("ERROR THIS SHOULD NOT RUN\n");
		exit(0);
	}
	else if(return_code > 0){
		printf("Parent Process: \n");
		int pid = return_code;
		int status = 0;
		printf("waiting\n");
		waitpid(pid, &status, 0);
		// char buffer[4000];
		// printf("reading from pipe...\n");
		// read(fds[0], buffer, 4);
		// printf("%s\n", buffer);
		dup2(fds[0], STDIN_FILENO);
		execlp("wc", "wc", NULL);
		printf("SHOULD NOT BE PRINTED");
		printf("Child process done, exited with code: %d\n", WEXITSTATUS(status));
	}
	else{
		printf("Child process creation error\n");
	}
	
	return 0;
}

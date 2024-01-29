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
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		printf("Run program A\n");
		execlp("ls", "ls", NULL);
		exit(0);
	}
	else if(return_code > 0){
		printf("Parent Process: \n");
		int pid = return_code;
		int status = 0;
		waitpid(pid, &status, 0);
		printf("Child process done, exited with code: %d\n", WEXITSTATUS(status));
		printf("Now run program B\n");
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		execlp("wc", "wc", NULL);
		printf("SHOULD NOT BE PRINTED");
		
	}
	else{
		printf("Child process creation error\n");
	}
	
	return 0;
}

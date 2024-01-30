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
		printf("Redirct and run program A\n");
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		// execlp("ls", "ls", NULL);
		printf("yoyoyo");
		exit(0);
	}
	else if(return_code > 0){
		printf("Parent Process: \n");
		int pid = return_code;
		int status = 0;
		waitpid(pid, &status, 0);
		printf("Child process done, exited with code: %d\n", WEXITSTATUS(status));
		printf("Now change stdin to be read end of pipe and run program B\n");
		char buf[100];
		read(fds[0], buf, 6);
		printf("%s\n", buf);
		// dup2(fds[0], STDIN_FILENO);
		// close(fds[0]);
		// execlp("wc", "wc", NULL);
		// printf("SHOULD NOT BE PRINTED");
		
	}
	else{
		printf("Child process creation error\n");
	}
	
	return 0;
}

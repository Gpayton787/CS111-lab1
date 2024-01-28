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
		close(fds[1]);
		close(0); //Close child processes stdin
		dup(fds[0]); //Point 0 fd to read end of pipe
		close(fds[0]); //Close unused readend fd
		printf("Now gonna call wc, hopefully reading from read end of pipe\n");
		execlp("wc", "wc", NULL);
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
		close(fds[0]); //close read end
		close(1); //Close stdout of parent
		dup(fds[1]); // point stdout to write end
		close(fds[1]); //close unused write end fd
		printf("gonna call ls now");
		execlp("ls", "ls", NULL);
		printf("SHOULD NOT BE PRINTED");
		printf("Child process done, exited with code: %d\n", WEXITSTATUS(status));
	}
	else{
		printf("Child process creation error\n");
	}
	
	return 0;
}

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
		close(fds[0]); //close read end
		close(1); //Close child processes stdout
		dup(fds[1]); //Point 1 fd to write end
		close(fds[1]); //Close unused write end
		printf("Now gonna call ls, hopefully writing to read end\n");
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
		close(fds[1]); //close write end
		close(0); //Close stdin of parent
		dup(fds[0]); // point stdin to read end
		close(fds[0]); //close unused read end fd.
		printf("gonna call wc now");
		execlp("wc", "wc", NULL);
		printf("SHOULD NOT BE PRINTED");
		printf("Child process done, exited with code: %d\n", WEXITSTATUS(status));
	}
	else{
		printf("Child process creation error\n");
	}
	
	return 0;
}

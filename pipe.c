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
		// execlp("ls", "ls", NULL);
		write(fds[1], "four", 4);
		printf("written\n");
		exit(0);
	}
	else if(return_code > 0){
		printf("Parent Process: \n");
		int pid = return_code;
		int status = 0;
		printf("waiting\n");
		waitpid(pid, &status, 0);
		char buffer[100];
		printf("reading\n");
		read(fds[0], buffer, 4);
		printf("%s\n", buffer);
		printf("Child process done, exited with code: %d\n", WEXITSTATUS(status));
	}
	else{
		printf("Child process creation error\n");
	}
	
	return 0;
}

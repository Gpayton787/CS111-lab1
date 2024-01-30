#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	
	// for(int i = 1; i < argc; i++){
	// 	//If we only have 1 program
	// 	if(argc == 2){
	// 		execlp(argv[i], argv[i], NULL);
	// 	}
	// }

	int fds[2];
	pipe(fds); //Create a pipe

	int return_code = fork();
	if(return_code == 0){
		//Child
		printf("Child Process: \n");
		printf("Redirect stdout and run program A\n");
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		execlp("ls", "ls", NULL);
		//break here
		printf("SHOULD NOT BE PRINTED");
		exit(0);
	}
	else if(return_code > 0){
		printf("Parent Process: \n");
		int pid = return_code;
		int status = 0;
		waitpid(pid, &status, 0);
		printf("Child process done, exited with code: %d\n", WEXITSTATUS(status));
		printf("Redirct stdin run program B\n");
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		execlp("cat", "cat", NULL);
		printf("SHOULD NOT BE PRINTED");
		
	}
	else{
		printf("Child process creation error\n");
	}
	
	return 0;
}

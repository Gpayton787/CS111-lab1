#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	//If no program
	if(argc == 1){
		exit(0);
	}
	//If we only have 1 program
	if(argc == 2){
		execlp(argv[1], argv[1], NULL);
	}

	for(int i = 1; i < argc-1; i++){
		//Create a new pipe
		int fds[2];
		pipe(fds);

		int return_code = fork();

		if(return_code == 0){
			//Child

			//Write to pipe
			dup2(fds[1], STDOUT_FILENO);
			close(fds[1]);
			close(fds[0]);
			execlp(argv[i], argv[i], NULL);
		}
		else if(return_code > 0){
			//Parent
			int pid = return_code;
			int status = 0;
			waitpid(pid, &status, 0);
			//Read from the pipe
			dup2(fds[0], STDIN_FILENO);
			close(fds[0]);
			close(fds[1]);
		}
		else{
			printf("Child Process Creation Error\n");
		}	
	}
	//Run the remaining program
	execlp(argv[0]);


	return 0;

	//Static 2 argument program

	int fds[2];
	pipe(fds); //Create a pipe

	int return_code = fork();
	if(return_code == 0){
		//Child
		printf("Child Process: \n");
		printf("Redirect stdout and run program A\n");
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		close(fds[0]);
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
		close(fds[1]);
		execlp("cat", "cat", NULL);
		printf("SHOULD NOT BE PRINTED");
		
	}
	else{
		printf("Child process creation error\n");
	}
	
	return 0;
}

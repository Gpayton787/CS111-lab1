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
		printf("execute %s\n", argv[1]);
		// execlp(argv[1], argv[1], NULL);
	}
	for(int i = 1; i < argc-1; i++){
		//Create pipes
		int fds1[2];
		int fds2[2];
		pipe(fds1);
		pipe(fds2);
		//If there are more than 2 processes left
		if(argc-i > 2){
			;
		}
		//Fork the process
		int return_code = fork();
		if(return_code == 0){
			//Program A
			printf("Child Process %d:\n", i);
			//If not the first process read from pipe 2
			if(i != 1){
				dup2(fds2[0], STDIN_FILENO);
				close(fds2[0]);
				close(fds2[1]);
				printf("reading from stdin...");
			}
			else{
				printf("not reading from stdin...");
			}
			//redirect its stdout to write end of pipe 1
			dup2(fds1[1], STDOUT_FILENO);
			//Close pipe ends to prevent hanging
			close(fds1[1]);
			close(fds1[0]);
			//Call program
			printf("results of program %d\n", i);
			// execlp(argv[i], argv[i], NULL);
		}
		else if(return_code > 0){
			printf("Parent Process %d:\n", i);
			int pid = return_code;
			int status = 0;
			waitpid(pid, &status, 0);
			//Take input from read end of pipe
			dup2(fds1[0], STDIN_FILENO);
			close(fds1[0]);
			close(fds1[1]);
			char buffer[4096];
    		read(STDIN_FILENO, buffer, 23);
			//If we are not the last program, redirect to write end of pipe 2
			if(i+1 != argc-1){
				dup2(fds2[1], STDOUT_FILENO);
				//Close pipe ends to prevent hanging
				close(fds2[1]);
				close(fds2[0]);
			}
			//Else we are the last program
			printf("%s", buffer);
			printf("+ program %i\n", i+1);
			// execlp(argv[i+1], argv[i+1], NULL);
		}
	}
	//If there is a program left over
	if(argc % 2){
		//Execute the last program reading from pipe 2
		;
	}

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

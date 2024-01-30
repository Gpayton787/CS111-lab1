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
		//Create pipe 1
		printf("Iteration: %i\n", i);
		int fds1[2];
		pipe(fds1);
		//If there are more than 2 processes left
		if(argc-i > 2){
			;
		}
		else{
			//There are only two processes left, pipe2 not needed
			int return_code = fork();
			if(return_code == 0){
				//Child
				
				//If this NOT the first process, take from read end of pipe2
				// if(i != 1){
				// 	dup2(fds2[0], STDIN_FILENO);
				// 	close(fds2[0]);
				// 	close(fds2[1]);
				// 	printf("reading from stdin...");
				// }
				//redirect its stdout to write end of pipe 1
				dup2(fds1[1], STDOUT_FILENO);
				close(fds1[1]);
				close(fds1[0]);
				//Call program
				execlp(argv[i], argv[i], NULL);
			}
			else if(return_code > 0){
				//Parent
				int pid = return_code;
				int status = 0;
				waitpid(pid, &status, 0);
				//Read from pipe 1
				dup2(fds1[0], STDIN_FILENO);
				close(fds1[0]);
				close(fds1[1]);
				//If we are not the last program, create pipe2 and write to it

				//Else we are the last program
				execlp(argv[i+1], argv[i+1], NULL);
			}
		}
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

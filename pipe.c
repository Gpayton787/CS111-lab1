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
	//Declare pipe2's fds so we can still access them outside of the loop
	for(int i = 1; i < argc; i+=2){
		int fds2[2];
		//If only one remaining process
		if(i+1 == argc){
			printf("ONE REMAINING PROCESS\n");
			dup2(fds2[0], STDIN_FILENO);
			close(fds2[1]);
			close(fds2[0]);
			execlp(argv[argc-1], argv[argc-1], NULL);
		}
		//Create pipe 1
		printf("Iteration: %i\n", i);
		int fds1[2];
		int retval = pipe(fds1);
		if(retval < 0){
			perror("pipe");
            exit(EXIT_FAILURE);
		}
		//Wait to create second pipe
		printf("There are %i remaining programs to be run\n", argc-i);
	
		int return_code = fork();
		if(return_code == 0){
			//Child
			// If this is NOT the first process, take from read end of pipe2
			if(i != 1){
				printf("reading from stdin...");
				dup2(fds2[0], STDIN_FILENO);
				close(fds2[1]);
				close(fds2[0]);
				printf("Sucessfully reading from stdin...\n");
			}
			//redirect its stdout to write end of pipe 1
			printf("Redirect Output, then call program\n");
			dup2(fds1[1], STDOUT_FILENO);
			close(fds1[1]);
			close(fds1[0]);
			//Call program
			execlp(argv[i], argv[i], NULL);
		}
		else if(return_code > 0){
			//Parent
			printf("Parent, wait\n");
			int pid = return_code;
			int status = 0;
			waitpid(pid, &status, 0);
			//Read from pipe 1
			printf("Take from stdin of pipe1\n");
			dup2(fds1[0], STDIN_FILENO);
			close(fds1[0]);
			close(fds1[1]);
			//If we are not the last program, create pipe2, fork, and write to it
			printf("%i is greater than 2?\n", argc-i);
			if(argc-i > 2){
				printf("Not the last program, create pipe 2\n");
				pipe(fds2);
				int return_code = fork();
				if(return_code == 0){
					//Child, execute program, write to pipe 2
					dup2(fds2[1], STDOUT_FILENO);
					close(fds2[1]);
					close(fds2[0]);
					execlp(argv[i+1], argv[i+1], NULL);
				}
				if(return_code > 0){
					//Do nothing just go to the next iteration
					int pid = return_code;
					int status = 0;
					waitpid(pid, &status, 0);
				}
			}
			else{
				printf("Last Program\n");
				//We are the last program
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

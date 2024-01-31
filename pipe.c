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
	execlp(argv[argc-1], argv[argc-1], NULL);

	exit(1);
	return 0;

}

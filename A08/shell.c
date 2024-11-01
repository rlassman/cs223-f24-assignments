/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 10/31/24
 * Description: Makes a shell
 ---------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main() {
        char* inp;
        pid_t pid;
        int status;
        char* argsv[10];

	using_history();
	stifle_history(500);

	char buff[100]; 
        getcwd(buff, 100);

	char prompt[200];
    	snprintf(prompt, sizeof(prompt), ANSI_COLOR_GREEN "Rebecca's shell: %s <3 " ANSI_COLOR_RESET, buff);

        while(1) {
                inp = readline(prompt);		
		add_history(inp);
	
                char* com = strtok(inp, " ");
                int i = 0;
                while(com != NULL) {
                        argsv[i] = com;
			com = strtok(NULL, " ");
			i++;
                }
		argsv[i] = NULL;

		if(strcmp(argsv[0], "exit") == 0) {
			free(inp);
 	                exit(1);
                }

		pid = fork();

                if(pid == 0) {
                        if(execvp(argsv[0], argsv) < 0) {
                                printf("%s: Command not found.\n", argsv[0]);
                                exit(1);
                        }
                } else {
                        waitpid(pid, &status, 0);
			if(status == 11) {
				printf("Oh no! Segmentation fault! Core dumped!");
			}
                }
		free(inp);
        }

}

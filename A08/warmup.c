/*----------------------------------------------
 * Author: Rebecca Lassman
 * Date: 10/31/24
 * Description: Forks based on the given diagram
 ---------------------------------------------*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	pid_t pid;
	int child_status;

	printf("%d] A0\n", getpid());

	pid = fork();
	if (pid == 0) {
		printf("%d] B1\n", getpid());
	} else {
		printf("%d] B0\n", getpid());
		pid = wait(&child_status);
		pid = fork();
		if (pid == 0) {
			printf("%d] C1\n", getpid());
		} else {
			printf("%d] C0\n", getpid());
		}
	}

	printf("%d] Bye\n", getpid());

	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int childpid;
	int status;
	switch (childpid = fork()) {
	case 0:
		// exit(-2);
        return -2;
	default:
		wait(&status);
		printf("Child status %d\n", WEXITSTATUS(status));
	}
}
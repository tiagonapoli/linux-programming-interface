#define _XOPEN_SOURCE 500
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int fd_is_valid(int fd) { return fcntl(fd, F_GETFD) != -1 || errno != EBADF; }

int main() {
	int v = 111;


    printf("wololo");
	if (vfork() == 0) {
		v *= 3;
		close(STDIN_FILENO);
        close(STDOUT_FILENO);
        // The exit will flush stdio buffers
        // Since this is userland memory and it's
        // shared with the parent we will lost
        // the "wololo" output buffered
        // (the stdio file descriptors are duped, just
        // as fork does, so the father will still be 
        // able to write to stdout).
        // Change this exit(0) to _exit(0) to see the
        // "wololo" output again
		exit(0);
	} else {
		printf("Father; v=%d\n", v);
		if (fd_is_valid(STDIN_FILENO)) {
			printf("STDIN is still valid\n");
		}

		if (fd_is_valid(STDOUT_FILENO)) {
			printf("STDOUT is still valid\n");
		}

		if (fd_is_valid(STDERR_FILENO)) {
			printf("STDERR is still valid\n");
		}
	}
}
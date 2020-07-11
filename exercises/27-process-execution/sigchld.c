#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../book-src/lib/tlpi_hdr.h"

void handler(int sig) { printf("Caught SIGCHLD\n"); }

int main() {
	struct sigaction sa;
	sigset_t mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &mask, NULL);

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sa.sa_flags = SA_SIGINFO;

	if (sigaction(SIGCHLD, &sa, NULL) != 0) {
		errExit("sigaction");
	}

	int child_pid;
	switch (child_pid = fork()) {
	case -1:
		errExit("fork");

	case 0:
		exit(EXIT_SUCCESS);
	}

	printf("Child pid: %d\n", child_pid);

	int res;
	if ((res = waitpid(child_pid, NULL, 0)) != child_pid) {
		errExit("waitpid");
	}

	printf("Pid waited: %d\n", res);
	sleep(2);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	sigprocmask(SIG_BLOCK, &mask, NULL);
	system("echo 'system was executed while blocking SIGCHLD'");
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	system("echo 'system was executed without blocking SIGCHLD'");
}
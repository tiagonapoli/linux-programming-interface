#define _GNU_SOURCE
#include <pthread.h>
#include <signal.h>
#include <wait.h>
#include <string.h>
#include <sys/wait.h>
#include "../../book-src/lib/tlpi_hdr.h"
#define THREADS 50

void printWaitStatus(const char *msg, int status) {
	if (msg != NULL) printf("%s", msg);

	if (WIFEXITED(status)) {
		printf("child exited, status=%d\n", WEXITSTATUS(status));

	} else if (WIFSIGNALED(status)) {
		printf("child killed by signal %d (%s)", WTERMSIG(status), strsignal(WTERMSIG(status)));
		if (WCOREDUMP(status)) printf(" (core dumped)");
		printf("\n");

	} else if (WIFSTOPPED(status)) {
		printf("child stopped by signal %d (%s)\n", WSTOPSIG(status), strsignal(WSTOPSIG(status)));

	} else if (WIFCONTINUED(status)) {
		printf("child continued\n");

	} else {
		printf("what happened to this child? (status=%x)\n", (unsigned int)status);
	}
}

void handler(int signo, siginfo_t *info, void *uctx) {
	int status;
	pid_t child;

	while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
		printf("[handler %ld]: Reaped child %d\n", pthread_self(), child);
		printWaitStatus(NULL, status);
	}
}

void *thread_handler(void *arg) {
	long id = (long)arg;
	if (id == 0) {
		printf("Will fork from %ld\n", pthread_self());
		int child;
		switch (child = fork()) {
		case -1:
			errExit("fork");
		case 0:
			printf("Child will exit\n");
			return 0;
		}
	}

	sleep(5);
}

int main() {
	pthread_t t[THREADS];
	int r;

	struct sigaction act;
	act.sa_sigaction = handler;
	act.sa_flags = SA_SIGINFO;
	sigemptyset(&act.sa_mask);

	if (sigaction(SIGCHLD, &act, NULL) == -1) {
		errExit("sigaction");
	}
	printf("Main thread %ld\n", pthread_self());

	for (long i = 0; i < THREADS; i++) {
		r = pthread_create(&t[i], NULL, thread_handler, (void *)i);
		if (r != 0) {
			errExitEN(r, "pthread_create");
		}
	}

	for (int i = 0; i < THREADS; i++) {
		r = pthread_join(t[i], NULL);
		if (r != 0) {
			errExitEN(r, "pthread_join");
		}
	}
}
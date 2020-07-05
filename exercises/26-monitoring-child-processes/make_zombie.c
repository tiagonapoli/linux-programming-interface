#define _GNU_SOURCE /* Get strsignal() declaration from <string.h> */
#define _POSIX_C_SOURCE 200809L
#include "../../book-src/lib/tlpi_hdr.h"
#include <signal.h>
#include <string.h>
#include <libgen.h>
#include <sys/wait.h>

#define CMD_SIZE 200

void printWaitStatus(const char *msg, siginfo_t *info) {
	if (msg != NULL) printf("%s", msg);
	switch (info->si_code) {
	case CLD_EXITED:
		printf("child exited, status=%d\n", info->si_status);
		break;
	case CLD_KILLED:
		printf("child killed by signal %d (%s)\n", info->si_status, strsignal(info->si_status));
		break;
	case CLD_DUMPED:
		printf("child killed by signal %d (%s)\n", info->si_status, strsignal(info->si_status));
		printf("core dumped\n");
		break;
	case CLD_STOPPED:
		printf("child stopped by signal %d (%s)\n", info->si_status, strsignal(info->si_status));
		break;
	case CLD_CONTINUED:
		printf("child continued\n");
		break;
	}

	printf("\n");
}

int main(int argc, char *argv[]) {
	char cmd[CMD_SIZE];
	pid_t childPid;
	siginfo_t si;

	setbuf(stdout, NULL); /* Disable buffering of stdout */
	printf("Parent PID=%ld\n", (long)getpid());

	switch (childPid = fork()) {
	case -1:
		errExit("fork");

	case 0: /* Child: immediately exits to become zombie */
		printf("Child (PID=%ld) exiting\n", (long)getpid());
		_exit(EXIT_SUCCESS);
	default: /* Parent */

		if (waitid(P_ALL, 0, &si, WEXITED | WNOWAIT) < 0) {
			errExit("waitid");
		}

		printWaitStatus(NULL, &si);
		snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
		system(cmd); /* View zombie child */

		/* Now send the "sure kill" signal to the zombie */

		if (kill(childPid, SIGKILL) == -1) errMsg("kill");
		sleep(3); /* Give child a chance to react to signal */
		printf("After sending SIGKILL to zombie (PID=%ld):\n", (long)childPid);
		system(cmd); /* View zombie child again */

		exit(EXIT_SUCCESS);
	}
}

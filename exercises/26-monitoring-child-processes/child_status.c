#define _GNU_SOURCE /* Get strsignal() declaration from <string.h> */
#define _POSIX_C_SOURCE 199309L
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include "../../book-src/lib/tlpi_hdr.h"

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
	siginfo_t si;

	if (argc > 1 && strcmp(argv[1], "--help") == 0) usageErr("%s [exit-status]\n", argv[0]);

	switch (fork()) {
	case -1:
		errExit("fork");

	case 0: /* Child: either exits immediately with given
	           status or loops waiting for signals */
		printf("Child started with PID = %ld\n", (long)getpid());
		if (argc > 1) /* Status supplied on command line? */
			exit(getInt(argv[1], 0, "exit-status"));
		else /* Otherwise, wait for signals */
			for (;;) pause();
		exit(EXIT_FAILURE); /* Not reached, but good practice */

	default: /* Parent: repeatedly wait on child until it
	            either exits or is terminated by a signal */
		for (;;) {
			if (waitid(P_ALL, 0, &si, WEXITED | WSTOPPED | WCONTINUED) == -1) errExit("waitid");

			/* Print status in hex, and as separate decimal bytes */

			printf("waitpid() returned: PID=%ld;\n", (long)si.si_pid);
			printWaitStatus(NULL, &si);

			if (si.si_code == CLD_EXITED || si.si_code == CLD_DUMPED || si.si_code == CLD_KILLED) {
				exit(EXIT_SUCCESS);
			}
		}
	}
}

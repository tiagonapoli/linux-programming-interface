#define _GNU_SOURCE
#include <signal.h>
#include <string.h>
#include "../../book-src/lib/tlpi_hdr.h"

void printSigset(FILE *of, const char *prefix, const sigset_t *sigset) {
	int sig, cnt;

	cnt = 0;
	for (sig = 1; sig < NSIG; sig++) {
		if (sigismember(sigset, sig)) {
			cnt++;
			fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
		}
	}

	if (cnt == 0) fprintf(of, "%s<empty signal set>\n", prefix);
}

int printSigMask(FILE *of, const char *msg) {
	sigset_t currMask;

	if (msg != NULL) fprintf(of, "%s", msg);

	if (sigprocmask(SIG_BLOCK, NULL, &currMask) == -1) return -1;

	printSigset(of, "\t\t", &currMask);

	return 0;
}

int printPendingSigs(FILE *of, const char *msg) {
	sigset_t pendingSigs;

	if (msg != NULL) fprintf(of, "%s", msg);

	if (sigpending(&pendingSigs) == -1) return -1;

	printSigset(of, "\t\t", &pendingSigs);

	return 0;
}

static void handler(int sig) { 
    printSigMask(stdout, "Inside handler sigmask"); 
    fprintf(stdout, "Executed SIGCONT handler!");
}

int main(int argc, char *argv[]) {
	int n, numSecs;
	sigset_t pendingMask;

	printf("%s: PID is %ld\n", argv[0], (long)getpid());

	struct sigaction opt;
	opt.sa_handler = handler;
	opt.sa_flags = 0;
	sigemptyset(&opt.sa_mask);
	sigaction(SIGCONT, &opt, NULL);

	sigset_t blockSigs;
	sigemptyset(&blockSigs);
	sigaddset(&blockSigs, SIGCONT);
	sigprocmask(SIG_BLOCK, &blockSigs, NULL);
	printSigMask(stdout, "SigMask on main:");

	for (int i = 0; i < 1000; i++) {
		if (sleep(10) != 0) {
			perror("sleep: ");
		}

		printPendingSigs(stdout, "Pending sigs: ");
		if (i == 5) {
			sigemptyset(&blockSigs);
			sigprocmask(SIG_SETMASK, &blockSigs, NULL);
			printSigMask(stdout, "Update sigmask on main:");
		}
	}

	exit(EXIT_SUCCESS);
}

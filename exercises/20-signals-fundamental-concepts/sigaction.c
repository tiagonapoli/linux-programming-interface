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

static int sigCnt[NSIG]; /* Counts deliveries of each signal */
static volatile sig_atomic_t gotSigint = 0;
/* Set nonzero if SIGINT is delivered */

static void handler(int sig) {
	if (sig == SIGINT)
		gotSigint = 1;
	else
		sigCnt[sig]++;
}

int main(int argc, char *argv[]) {
	int n, numSecs;
	sigset_t pendingMask, blockingMask, emptyMask;

	printf("%s: PID is %ld\n", argv[0], (long)getpid());

	for (n = 1; n < NSIG; n++) {
        struct sigaction opt;
        opt.sa_handler = handler;
        opt.sa_flags = 0;
        sigemptyset(&opt.sa_mask);
		sigaction(n, &opt, NULL);
	}

	if (argc > 1) {
		numSecs = getInt(argv[1], GN_GT_0, NULL);

		sigfillset(&blockingMask);
		if (sigprocmask(SIG_SETMASK, &blockingMask, NULL) == -1) errExit("sigprocmask");

		printf("%s: sleeping for %d seconds\n", argv[0], numSecs);
		sleep(numSecs);

		if (sigpending(&pendingMask) == -1) errExit("sigpending");

		printf("%s: pending signals are: \n", argv[0]);
		printSigset(stdout, "\t\t", &pendingMask);

		sigemptyset(&emptyMask); /* Unblock all signals */
		if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1) errExit("sigprocmask");
	}

	while (!gotSigint) /* Loop until SIGINT caught */
		continue;

	for (n = 1; n < NSIG; n++) /* Display number of signals received */
		if (sigCnt[n] != 0) printf("%s: signal %d caught %d time%s\n", argv[0], n, sigCnt[n], (sigCnt[n] == 1) ? "" : "s");

	exit(EXIT_SUCCESS);
}

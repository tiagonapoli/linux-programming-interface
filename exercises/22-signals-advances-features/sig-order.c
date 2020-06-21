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

__sig_atomic_t first = 0;

static void handler(int sig) {
	if (first == 0) {
		first = 1;
	}

	fprintf(stdout, "Executed normal signal %d\n", sig);
}

static void realtimeHandler(int sig, siginfo_t *info, void *ucontext) {
	if (first == 0) {
		first = 2;
	}

	fprintf(stdout, "Executed realtime signal %d. From pid: %d. Data: %d\n", sig, info->si_pid, info->si_int);
}

int main(int argc, char *argv[]) {
	int n, numSecs;
	sigset_t pendingMask;
	union sigval sv;

	printf("%s: PID is %ld\n", argv[0], (long)getpid());

	for (n = 1; n < NSIG; n++) {
		struct sigaction opt;
		opt.sa_handler = handler;
		opt.sa_flags = 0;
		sigemptyset(&opt.sa_mask);
		sigaction(n, &opt, NULL);
	}

	for (n = SIGRTMIN; n < SIGRTMAX; n++) {
		struct sigaction opt;
		opt.sa_sigaction = realtimeHandler;
		opt.sa_flags = SA_SIGINFO;
		sigemptyset(&opt.sa_mask);
		sigaction(n, &opt, NULL);
	}

	sigset_t blockSigs;
	sigfillset(&blockSigs);
	sigprocmask(SIG_SETMASK, &blockSigs, NULL);
	printSigMask(stdout, "SigMask on main:");

	if (raise(SIGINT) != 0) {
		errExit("raise");
	}

	sv.sival_int = 123;
	if (sigqueue(getpid(), SIGRTMIN, sv) < 0) {
		errExit("sigqueue");
	}

	sv.sival_int = 234;
	if (sigqueue(getpid(), SIGRTMIN, sv) < 0) {
		errExit("sigqueue");
	}

	sv.sival_int = 345;
	if (sigqueue(getpid(), SIGRTMIN + 1, sv) < 0) {
		errExit("sigqueue");
	}

	if (raise(SIGUSR2) != 0) {
		errExit("raise");
	}

	printPendingSigs(stdout, "Pending signals:");
	sleep(2);

	sigemptyset(&blockSigs);
	sigprocmask(SIG_SETMASK, &blockSigs, NULL);

	if (first == 2) {
		printf("Realtime executed first\n");
	} else if (first == 1) {
		printf("Regular executed first\n");
	}

	exit(EXIT_SUCCESS);
}

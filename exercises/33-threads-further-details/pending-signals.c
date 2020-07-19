#define _GNU_SOURCE
#include <pthread.h>
#include <signal.h>
#include "../../book-src/lib/tlpi_hdr.h"

void printSigset(const char *prefix, const sigset_t *sigset) {
	int sig, cnt;

	cnt = 0;
	for (sig = 1; sig < NSIG; sig++) {
		if (sigismember(sigset, sig)) {
			cnt++;
			printf("%s%d (%s)\n", prefix, sig, strsignal(sig));
		}
	}

	if (cnt == 0) printf("%s<empty signal set>\n", prefix);
}

int printSigMask(const char *msg) {
	sigset_t currMask;

	if (msg != NULL) printf("%s [%ld]", msg, pthread_self());

	if (pthread_sigmask(SIG_BLOCK, NULL, &currMask) == -1) return -1;

	printSigset("\t\t", &currMask);

	return 0;
}

int printPendingSigs(const char *msg) {
	sigset_t pendingSigs;

	if (msg != NULL) printf("%s", msg);

	if (sigpending(&pendingSigs) == -1) return -1;

	char prefix[20];
	sprintf(prefix, "[%ld]\t\t", pthread_self());
	printSigset(prefix, &pendingSigs);

	return 0;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_signal(void *arg) {
	int signo = (int)((long)arg);
	sleep(1);

	int r;
	r = pthread_mutex_lock(&mutex);
	if (r != 0) {
		errExitEN(r, "pthread_mutex_lock");
	}
	printSigMask("sigmask");
	r = pthread_mutex_unlock(&mutex);
	if (r != 0) {
		errExitEN(r, "pthread_mutex_unlock");
	}

	sleep(2);
	printPendingSigs("pending sigs");
}

int main() {
	pthread_t t[2];
	int r;

	sigset_t oldmask, mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	pthread_sigmask(SIG_BLOCK, &mask, &oldmask);
	r = pthread_create(&t[0], NULL, &thread_signal, (void *)SIGUSR1);
	if (r != 0) {
		errExitEN(r, "pthread_create");
	}

	pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR2);
	pthread_sigmask(SIG_BLOCK, &mask, NULL);

	r = pthread_create(&t[1], NULL, &thread_signal, (void *)SIGUSR2);
	if (r != 0) {
		errExitEN(r, "pthread_create");
	}

	pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
	pthread_kill(t[0], SIGUSR1);
	pthread_kill(t[1], SIGUSR2);

	for (int i = 0; i < 2; i++) {
		r = pthread_join(t[i], NULL);
		if (r != 0) {
			errExitEN(r, "pthread_join");
		}
	}
}
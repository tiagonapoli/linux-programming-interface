#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <time.h>
#include "../../book-src/lib/tlpi_hdr.h"

void handler(int signal, siginfo_t *info, void *ucontext) { printf("Signal received: %d [timer %p]\n", signal, info->si_value.sival_ptr); }

int main() {
	timer_t timerid;
	struct sigaction act;
	sigset_t mask;

	sigemptyset(&mask);
	act.sa_mask = mask;
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handler;
	sigaction(SIGALRM, &act, NULL);

	if (timer_create(CLOCK_REALTIME, NULL, &timerid) < 0) {
		errExit("timer_create");
	}

	printf("TimerID %p\n", timerid);

	struct itimerspec ts;
	ts.it_interval.tv_nsec = 0;
	ts.it_interval.tv_sec = 1;
	ts.it_value.tv_nsec = 0;
	ts.it_value.tv_sec = 1;
	if (timer_settime(timerid, 0, &ts, NULL) < 0) {
		errExit("timer_settime");
	}

	pause();
}
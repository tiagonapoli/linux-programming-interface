/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 23-5 */

/* ptmr_sigev_signal.c

   This program demonstrates the use of signals as the notification mechanism
   for expirations of a POSIX timer. Each of the program's command-line
   arguments specifies the initial value and interval for a POSIX timer. The
   format of these arguments is defined by the function itimerspecFromStr().

   The program establishes a handler for the timer notification signal, creates
   and arms one timer for each command-line argument, and then pauses. Each
   timer expiration causes the generation of a signal, and, when invoked, the
   signal handler displays information about the timer expiration.

   Kernel support for Linux timers is provided since Linux 2.6. On older
   systems, an incomplete user-space implementation of POSIX timers
   was provided in glibc.
*/
#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "../../book-src/lib/tlpi_hdr.h"

#define TIMER_SIG SIGRTMAX /* Our timer notification signal */
#define BUF_SIZE 1000

/* Convert a string of the following form to an itimerspec structure:
   "value.sec[/value.nanosec][:interval.sec[/interval.nanosec]]".
   Optional components that are omitted cause 0 to be assigned to the
   corresponding structure fields. */

void itimerspecFromStr(char *str, struct itimerspec *tsp) {
	char *dupstr, *cptr, *sptr;

	dupstr = strdup(str);

	cptr = strchr(dupstr, ':');
	if (cptr != NULL) *cptr = '\0';

	sptr = strchr(dupstr, '/');
	if (sptr != NULL) *sptr = '\0';

	tsp->it_value.tv_sec = atoi(dupstr);
	tsp->it_value.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;

	if (cptr == NULL) {
		tsp->it_interval.tv_sec = 0;
		tsp->it_interval.tv_nsec = 0;
	} else {
		sptr = strchr(cptr + 1, '/');
		if (sptr != NULL) *sptr = '\0';
		tsp->it_interval.tv_sec = atoi(cptr + 1);
		tsp->it_interval.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;
	}
	free(dupstr);
}

/* Return a string containing the current time formatted according to
   the specification in 'format' (see strftime(3) for specifiers).
   If 'format' is NULL, we use "%c" as a specifier (which gives the'
   date and time as for ctime(3), but without the trailing newline).
   Returns NULL on error. */

char *currTime(const char *format) {
	static char buf[BUF_SIZE]; /* Nonreentrant */
	time_t t;
	size_t s;
	struct tm *tm;

	t = time(NULL);
	tm = localtime(&t);
	if (tm == NULL) return NULL;

	s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);

	return (s == 0) ? NULL : buf;
}

int main(int argc, char *argv[]) {
	struct itimerspec ts;
	struct sigaction sa;
	struct sigevent sev;
	timer_t *tidlist;
	int j;

	if (argc < 2) usageErr("%s secs[/nsecs][:int-secs[/int-nsecs]]...\n", argv[0]);

	tidlist = calloc(argc - 1, sizeof(timer_t));
	if (tidlist == NULL) errExit("malloc");

	/* Create and start one timer for each command-line argument */

	sev.sigev_notify = SIGEV_SIGNAL; /* Notify via signal */
	sev.sigev_signo = TIMER_SIG;     /* Notify using this signal */

	for (j = 0; j < argc - 1; j++) {
		itimerspecFromStr(argv[j + 1], &ts);

		sev.sigev_value.sival_ptr = &tidlist[j];
		/* Allows handler to get ID of this timer */

		if (timer_create(CLOCK_REALTIME, &sev, &tidlist[j]) == -1) errExit("timer_create");
		printf("Timer ID: %ld (%s)\n", (long)tidlist[j], argv[j + 1]);

		if (timer_settime(tidlist[j], 0, &ts, NULL) == -1) errExit("timer_settime");
	}

	siginfo_t si;
	sigset_t signalsToWait;
    sigemptyset(&signalsToWait);
	sigaddset(&signalsToWait, TIMER_SIG);
    sigprocmask(SIG_BLOCK, &signalsToWait, NULL);
    printf("TimerSIG: %d [min %d]\n", TIMER_SIG, SIGRTMIN);
	for (;;) {
		int s = sigwaitinfo(&signalsToWait, &si);
        if(s < 0) {
            errExit("sigwaitinfo");
        }

		timer_t *tidptr;
		tidptr = si.si_value.sival_ptr;
		printf("[%s] Got signal %d\n", currTime("%T"), s);
		printf("    *sival_ptr         = %ld\n", (long)*tidptr);
		printf("    timer_getoverrun() = %d\n", timer_getoverrun(*tidptr));
	}
}

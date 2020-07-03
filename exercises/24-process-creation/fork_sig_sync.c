/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 24-6 */

/* fork_sig_sync.c

   Demonstrate how signals can be used to synchronize the actions
   of a parent and child process.
*/
#define XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <time.h>
#include "../../book-src/lib/tlpi_hdr.h"

#define SYNC_SIG SIGUSR1 /* Synchronization signal */

static void /* Signal handler - does nothing but return */
handler(int sig) {}

#define BUF_SIZE 1000

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
	pid_t childPid;
	sigset_t blockMask, origMask, emptyMask;
	struct sigaction sa;

	setbuf(stdout, NULL); /* Disable buffering of stdout */

	sigemptyset(&blockMask);
	sigaddset(&blockMask, SYNC_SIG); /* Block signal */
	if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1) errExit("sigprocmask");

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	if (sigaction(SYNC_SIG, &sa, NULL) == -1) errExit("sigaction");

	switch (childPid = fork()) {
	case -1:
		errExit("fork");

	case 0: /* Child */

		/* Child does some required action here... */

		printf("[%s %ld] Child started - doing some work\n", currTime("%T"), (long)getpid());
		sleep(2); /* Simulate time spent doing some work */

		/* And then signals parent that it's done */

		printf("[%s %ld] Child about to signal parent\n", currTime("%T"), (long)getpid());
		if (kill(getppid(), SYNC_SIG) == -1) errExit("kill");

		/* Now child can do other things... */

		_exit(EXIT_SUCCESS);

	default: /* Parent */

		/* Parent may do some work here, and then waits for child to
		   complete the required action */

		printf("[%s %ld] Parent about to wait for signal\n", currTime("%T"), (long)getpid());
		sigemptyset(&emptyMask);
		if (sigsuspend(&emptyMask) == -1 && errno != EINTR) errExit("sigsuspend");
		printf("[%s %ld] Parent got signal\n", currTime("%T"), (long)getpid());

		/* If required, return signal mask to its original state */

		if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1) errExit("sigprocmask");

		/* Parent carries on to do other things... */

		exit(EXIT_SUCCESS);
	}
}

#define _GNU_SOURCE
#include <signal.h>
#include <string.h>
#include "../../book-src/lib/tlpi_hdr.h"

static int handlerCount = 0;
static int shouldCont = 1;

int mysiginterrupt(int sig, int flag) {
	struct sigaction opt;
	if (sigaction(sig, NULL, &opt) < 0) {
		errExit("sigaction");
	}

	if (flag == 0) {
		opt.sa_flags |= SA_RESTART;
	} else {
		opt.sa_flags &= ~SA_RESTART;
	}

	if (sigaction(sig, &opt, NULL) < 0) {
		errExit("sigaction");
	}

    return 0;
}

static void handler(int sig) {
	handlerCount += 1;
	int id = handlerCount;
	printf("Started handler\n");
	sleep(5);
	shouldCont = 0;
	printf("Done handler %d\n", id);
}

int main(int argc, char *argv[]) {
	printf("%s: PID is %ld\n", argv[0], (long)getpid());

	struct sigaction opt;

	if (sigaction(SIGINT, NULL, &opt) < 0) {
		errExit("sigaction");
	}

	printf("before sigaction: %p 0x%04x\n", opt.sa_handler, opt.sa_flags);

	opt.sa_handler = handler;
	opt.sa_flags = SA_RESTART;
	sigemptyset(&opt.sa_mask);
	if (sigaction(SIGINT, &opt, NULL) < 0) {
		errExit("sigaction");
	}

	printf("SA_RESTART          : 0x%08x 0x%08x\n", SA_RESTART, ~SA_RESTART);
	if (sigaction(SIGINT, NULL, &opt) < 0) {
		errExit("sigaction");
	}

	printf("before siginterrupt 1: 0x%08x %p\n", opt.sa_flags, opt.sa_handler);

	if (mysiginterrupt(SIGINT, 1) < 0) {
		errExit("siginterrupt");
	}

	if (sigaction(SIGINT, NULL, &opt) < 0) {
		errExit("sigaction");
	}

	printf("after siginterrupt 1 : 0x%08x %p\n", opt.sa_flags, opt.sa_handler);

	if (mysiginterrupt(SIGINT, 0) < 0) {
		errExit("siginterrupt");
	}

	if (sigaction(SIGINT, NULL, &opt) < 0) {
		errExit("sigaction");
	}

	printf("after siginterrupt 0 : 0x%08x %p\n", opt.sa_flags, opt.sa_handler);
}

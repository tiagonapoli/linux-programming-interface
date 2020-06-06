#define _GNU_SOURCE
#include <signal.h>
#include <string.h>
#include "../../book-src/lib/tlpi_hdr.h"

static int handlerCount = 0;

static void handler(int sig) {
	handlerCount += 1;
	int id = handlerCount;
	printf("Started handler\n");
	sleep(5);
	printf("Done handler %d\n", id);
}

int main(int argc, char *argv[]) {
	printf("%s: PID is %ld\n", argv[0], (long)getpid());

	struct sigaction opt;
	opt.sa_handler = handler;
	opt.sa_flags = SA_RESETHAND;
	sigemptyset(&opt.sa_mask);
	if (sigaction(SIGINT, &opt, NULL) < 0) {
		errExit("sigaction");
	}

	while (true) {
	}
	printf("Executed handler %d times\n", handlerCount);
}

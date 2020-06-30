#include "../../book-src/lib/tlpi_hdr.h"
#include <sys/time.h>
#include <time.h>

static void displayTimes(Boolean showTimer) {
	static int callNum = 0;
	static struct timeval start;
	struct itimerval itv;
	struct timeval curr;

	if (callNum == 0) {
		if (gettimeofday(&start, NULL) < 0) {
			errExit("gettimeofday");
		}
	}

	if (gettimeofday(&curr, NULL) < 0) {
		errExit("gettimeofday");
	}

	printf("%6s %6.2f\n", "", curr.tv_sec - start.tv_sec + (curr.tv_usec - start.tv_usec) / 1000000.0);

	if (showTimer) {
		if (getitimer(ITIMER_REAL, &itv) < 0) {
			errExit("getitimer");
		}
		printf("Timer: %6s %6.2f\n", "", itv.it_value.tv_sec + itv.it_value.tv_usec / 1000000.0);
	}

	callNum++;
}

int alarm2(int seconds) {
	struct itimerval tv;
	tv.it_interval.tv_sec = 0;
	tv.it_interval.tv_usec = 0;

	tv.it_value.tv_sec = seconds;
	tv.it_value.tv_usec = 0;
	return setitimer(ITIMER_REAL, &tv, NULL);
}

int main() {
	displayTimes(FALSE);
	int ini = alarm2(10);
	while (1) {
		clock_t prevClock = clock();
		while ((clock() - prevClock) / (double)CLOCKS_PER_SEC < 0.5) {
		}
		displayTimes(TRUE);
	}
}
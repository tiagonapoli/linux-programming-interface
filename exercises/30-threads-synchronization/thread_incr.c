#include <pthread.h>
#include "../../book-src/lib/tlpi_hdr.h"

/* "volatile" prevents compiler optimizations of arithmetic operations on 'glob' */
static volatile int glob = 0;

struct threadData {
	int id;
	int loops;
};

static void *threadFunc(void *arg) {
	struct threadData *data = (struct threadData *)arg;
	int loc, j;

	for (j = 0; j < data->loops; j++) {
		loc = glob;
		loc++;
		glob = loc;
		printf("[%d] %d\n", data->id, glob);
	}

	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t t1, t2;
	int loops, s;

	loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

	struct threadData data1, data2;

	data1.loops = data2.loops = loops;
	data1.id = 1;
	data2.id = 2;
	s = pthread_create(&t1, NULL, threadFunc, &data1);
	if (s != 0) errExitEN(s, "pthread_create");
	s = pthread_create(&t2, NULL, threadFunc, &data2);
	if (s != 0) errExitEN(s, "pthread_create");

	s = pthread_join(t1, NULL);
	if (s != 0) errExitEN(s, "pthread_join");
	s = pthread_join(t2, NULL);
	if (s != 0) errExitEN(s, "pthread_join");

	printf("glob = %d\n", glob);
	exit(EXIT_SUCCESS);
}

#include <pthread.h>
#include "../../book-src/lib/tlpi_hdr.h"

void *threadFn(void *arg) {
	pthread_t tid = pthread_self();

	int s = pthread_join(tid, NULL);
	if (s != 0) {
		errExitEN(s, "pthread_join");
	}

	// Solution
	// if (!pthread_equal(pthread_self(), tid)) {
	// 	printf("tid is different from self tid\n");
	// 	int s = pthread_join(tid, NULL);
	// 	if (s != 0) {
	// 		errExitEN(s, "pthread_join");
	// 	}

	// 	printf("pthread_join was successful\n");
	// }
}

int main() {
	pthread_t t1;

	int s = pthread_create(&t1, NULL, threadFn, NULL);
	if (s != 0) {
		errExitEN(s, "pthread_create");
	}

	s = pthread_join(t1, NULL);
	if (s != 0) {
		errExitEN(s, "pthread_join");
	}
}
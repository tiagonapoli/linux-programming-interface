#include <pthread.h>
#include "../../book-src/lib/tlpi_hdr.h"

typedef struct {
	pthread_mutex_t mutex;
	volatile Boolean done;
} one_time_init_control;

int one_time_init(one_time_init_control *control, void (*initFn)()) {
	if (!control->done) {
		printf("check on thread %ld\n", pthread_self());
		int r;
		r = pthread_mutex_lock(&control->mutex);
		if (r != 0) {
			errExitEN(r, "pthreads_mutex_lock");
		}

		if (!control->done) {
			(*initFn)();
			control->done = TRUE;
		}

		r = pthread_mutex_unlock(&control->mutex);
		if (r != 0) {
			errExitEN(r, "pthreads_mutex_unlock");
		}
	}
}

static one_time_init_control init_control = {.mutex = PTHREAD_MUTEX_INITIALIZER, .done = FALSE};

void init() { printf("Init on thread %ld\n", pthread_self()); }

void *threadFn(void *arg) {
	printf("Executing thread %ld\n", pthread_self());
	sleep(1);
	one_time_init(&init_control, init);
	one_time_init(&init_control, init);
	one_time_init(&init_control, init);
	one_time_init(&init_control, init);
	one_time_init(&init_control, init);
}

int main() {
	const int threads = 50;
	pthread_t t[threads];
	int r;

	for (long i = 0; i < threads; i++) {
		r = pthread_create(&t[i], NULL, threadFn, (void *)i);
		if (r != 0) {
			errExitEN(r, "pthread_create");
		}
	}

	for (int i = 0; i < threads; i++) {
		r = pthread_join(t[i], NULL);
		if (r != 0) {
			errExitEN(r, "pthread_join");
		}
	}
}
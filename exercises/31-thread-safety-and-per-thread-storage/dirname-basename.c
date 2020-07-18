#include <pthread.h>
#include "../../book-src/lib/tlpi_hdr.h"

static pthread_once_t basename_once = PTHREAD_ONCE_INIT;
static pthread_key_t basename_key;

static void destructor(void *buf) { free(buf); }

void create_basename_key() {
	int r = pthread_key_create(&basename_key, destructor);
	if (r != 0) {
		errExitEN(r, "pthread_key_create");
	}
}

char *t_safe_basename(char *path) {
	int r;
	char *buf;

	r = pthread_once(&basename_once, create_basename_key);
	if (r != 0) {
		errExitEN(r, "pthread_once");
	}

	buf = pthread_getspecific(basename_key);
	if (buf == NULL) {
		buf = malloc(1000 * sizeof(char));
		if (buf == NULL) {
			errExit("malloc");
		}

		r = pthread_setspecific(basename_key, buf);
		if (r != 0) {
			errExitEN(r, "pthread_setspecific");
		}
	}

	int l = strlen(path);
	int sz = 0;
	for (int i = l - 1; i >= 0; i--) {
		if (path[i] == '/') {
			break;
		}

		buf[sz++] = path[i];
	}

	buf[sz] = '\0';

	for (int i = 0; i < sz-i; i++) {
		buf[i] ^= buf[sz - i - 1] ^= buf[i] ^= buf[sz - i - 1];
	}

	return buf;
}

static pthread_once_t dirname_once = PTHREAD_ONCE_INIT;
static pthread_key_t dirname_key;

void create_dirname_key() {
	int r = pthread_key_create(&dirname_key, destructor);
	if (r != 0) {
		errExitEN(r, "pthread_key_create");
	}
}

char *t_safe_dirname(char *path) {
	int r;
	char *buf;

	r = pthread_once(&dirname_once, create_dirname_key);
	if (r != 0) {
		errExitEN(r, "pthread_once");
	}

	buf = pthread_getspecific(dirname_key);
	if (buf == NULL) {
		buf = malloc(1000 * sizeof(char));
		if (buf == NULL) {
			errExit("malloc");
		}

		r = pthread_setspecific(dirname_key, buf);
		if (r != 0) {
			errExitEN(r, "pthread_setspecific");
		}
	}

	strcpy(buf, path);
	int len = strlen(buf);
	for (int i = len - 1; i >= 0; i--) {
		if (buf[i] == '/') {
			buf[i] = '\0';
			break;
		}
	}

	return buf;
}

void *test(void *arg) {
	char path[100];
	int id = (unsigned long)arg;
	printf("ThreadID = %d\n", id);

	sprintf(path, "a/%d/c/%d", id, id);
	printf("[%d] path = [%s]\n", id, path);
	printf("[%d] basename = [%s]\n", id, t_safe_basename(path));
	printf("[%d] dirname = [%s]\n", id, t_safe_dirname(path));
}

int main() {
	int r;

	const int threads = 20;
	pthread_t t[threads];

	for (unsigned long i = 0; i < threads; i++) {
		r = pthread_create(&t[i], NULL, test, (void *)i);
		if (r != 0) {
			errExitEN(r, "pthread_create");
		}
	}

	void *ret;
	for (int i = 0; i < threads; i++) {
		r = pthread_join(t[i], &ret);
		if (r != 0) {
			errExitEN(r, "pthread_join");
		}

		printf("Thread %d terminated\n", i);
	}

	printf("done\n");
}
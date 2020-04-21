#define _XOPEN_SOURCE
#include "../../book-src/lib/tlpi_hdr.h"
#include <string.h>
#include <stdlib.h>

static void listenv() {
	extern char **environ;
	char **p;
	printf("Environment variables\n");
	for (p = environ; *p != NULL; p++) {
		printf("%s\n", *p);
	}
	printf("\n\n");
}

static int myunsetenv(const char *name) {
	extern char **environ;
	char **p, **p1;
	int len;

	len = strlen(name);

	for (p = environ; *p != NULL; p++) {
		if (strncmp(name, *p, len) == 0 && (*p)[len] == '=') {
			*p = *(p + 1);
			for (p1 = p; *p1 != NULL; p1++) {
				*p1 = *(p1 + 1);
			}
		}
	}
}

static int mysetenv(const char *name, const char *value, int overwrite) {
	char *buf;
	char **p;
	int szName, szVal, i;

	if (!overwrite && getenv(name) != NULL) {
		return 0;
	}

	szName = strlen(name);
	szVal = strlen(value);
	buf = malloc(strlen(name) + strlen(value) + 2);
	if (buf == NULL) return -1;

	memcpy(buf, name, szName);
	buf[szName] = '=';
	memcpy(buf + szName + 1, value, szVal);
	buf[szName + szVal + 1] = '\0';

	myunsetenv(name);
	return putenv(buf);
}

int main() {
	myunsetenv("asdf");
	mysetenv("SHELL", "123", 1);
	mysetenv("asdf", "123", 1);

	listenv();
}
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <unistd.h>

extern char **environ;

int myexeclp(const char *file, ...) {
	// Copy args
	char *args[1000];
	int n = 0;
	va_list arg;
	va_start(arg, file);

	char filename[1000];
	strcpy(filename, file);
	args[n++] = basename(filename);
	args[n] = va_arg(arg, char *);
	while (args[n] != NULL) {
		args[++n] = va_arg(arg, char *);
	}
	va_end(arg);

	// Copy environment variables
	char *envp[1000];
	int envn = 0;
	for (char **e = environ; *e != NULL; e++, envn++) {
		envp[envn] = *e;
	}
	envp[envn] = NULL;

	int fileLen = strlen(file);
	for (int i = 0; i < fileLen; i++) {
		if (file[i] == '/') {
			return execve(file, args, envp);
		}
	}

	// Try exec each path
	char filePath[1000];
	char *path = getenv("PATH");
	char *pathPrefix;
	int ret;

	pathPrefix = strtok(path, ":");
	while (pathPrefix != NULL) {
		strcpy(filePath, pathPrefix);
		strcat(filePath, "/");
		strcat(filePath, file);
		if ((ret = execve(filePath, args, envp)) != 0 && errno != ENOENT && errno != EACCES) {
			return ret;
		}
		pathPrefix = strtok(NULL, ":");
	}

	return ret;
}

int main() {
	if (myexeclp("ls", "-l", NULL) != 0) {
		perror("execlp");
	}
}
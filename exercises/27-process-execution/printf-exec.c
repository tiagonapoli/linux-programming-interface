#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	printf("Hello world");
	execlp("sleep", "sleep", "0", (char *)NULL);
}
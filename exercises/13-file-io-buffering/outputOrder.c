#include <stdio.h>
#include <unistd.h>
#include <stdio_ext.h>

int main() {
	// setvbuf(stdout, NULL, _IONBF, 0);
	printf("If I had more time, \n");
	write(STDOUT_FILENO, "I would have written you a shorter letter.\n", 43);
}
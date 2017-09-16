
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>

#include "sys_calls.h"

int main(int argc, char *argv[]) {
	os_init();
	int status;
	if (fork() != 0) {
		waitpid(-1, &status, 0);
	} else {
		execv("shell/shell", argv);
	}
	return 0;
}

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "../src/sys_calls.h"

extern char *strtok_r(char *str, const char *delim, char **saveptr);

static int seq(int argc, char *argv[]) {
	int status;
	if (fork() != 0) {
		waitpid(-1, &status, 0);
	} else {
		execv("/usr/bin/seq", argv);
		exit(0);
	}
	return status;
}
static const struct {
	const char *name;
	int(*fn)(int, char *[]);
} app_list[] = {
	{ "seq", seq },
};

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

static int do_task(char *command) {
	char *saveptr;
	char *arg = strtok_r(command, " ", &saveptr);
	char *argv[256];
	int argc = 0;
	while (arg) {
		argv[argc++] = arg;
		arg = strtok_r(NULL, " ", &saveptr);
	}

	for (int i = 0; i < ARRAY_SIZE(app_list); ++i) {
		if (!strcmp(argv[0], app_list[i].name)) {
			return app_list[i].fn(argc, argv);
		}
	}

	char msg[256] = "No such function: ";
	strcat(msg, argv[0]);
	strcat(msg, "\n");
	os_sys_write(msg);
	return 1;
}

int main(int argc, char *argv[]) {
	while (1) {
		os_sys_write("> ");	
		char buffer[256];
		int bytes = os_sys_read(buffer, sizeof(buffer));
		if (!bytes) {
			break;
		}

		if (bytes < sizeof(buffer)) {
			buffer[bytes] = '\0';
		}

		char *saveptr;
		const char *comsep = "\n;";
		char *cmd = strtok_r(buffer, comsep, &saveptr);
		while (cmd) {
			do_task(cmd);
			cmd = strtok_r(NULL, comsep, &saveptr);
		}
	}
	os_sys_write("\n");
	return 0;
}

#include "os.h"
#include "my_string.h"

#define TRUE 1
#define FALSE 0
#define NULL ((void*) 0)
#define EOF (-1)

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

#define SHELL_INTRO "Welcome to shell!\n"
#define SHELL_OUTRO "Good bye!\n"
#define SHELL_WRONG_INPUT "Application or command not found.\n"

static const struct {
	const char *name;
	const char *path;
} app_list[] = {
	{ "echo", "/bin/echo" },
	{ "grep", "/bin/grep"},
	{ "seq", "/usr/bin/seq"},
};

int shell() {
    if (-1 == os_sys_write(SHELL_INTRO)) {
		perror("write");
	}
	while (TRUE) {
		if (-1 == os_sys_write("> ")) {
			perror("write");
		}
		char buf[256];
		int actual_size = os_sys_read(buf, sizeof(buf));
		if (-1 == actual_size) {
			perror("read");
		}

		if (!actual_size) {
			if (-1 == os_sys_write(SHELL_OUTRO)) {
				perror("write");
			}
			return 0;
		}

		if (actual_size < 256) {
			buf[actual_size] = '\0'; 
		}
		
		int pipefd[2];
		int in_fd = 0;
		char *symbol = buf;
		char *task = buf;
		while (*symbol != '\0') {
			switch (*symbol) {
				case ';' :
				case '\n' :
					*symbol = '\0';
					if (-1 == run_task(task, in_fd, 1)) {
						perror("run_task");
					}
					in_fd = 0;
					symbol++;
					task = symbol;
					continue;
				case '|' :
					*symbol = '\0';
					if (-1 == os_sys_pipe(pipefd)) {
						perror("pipe");
					}
					if (-1 == run_task(task, in_fd, pipefd[1])) {
						perror("run_task");
					}
					if (-1 == os_sys_close(pipefd[1])) {
						perror("close");
					}
					in_fd = pipefd[0];
					symbol++;
					task = symbol;
					continue;
			}
			symbol++;
		}
	}
	if (-1 == os_sys_write(SHELL_OUTRO)) {
		perror("write");
	}
	return 0;
}

int run_task(char *task, int in_fd, int out_fd) {
	char *symbol = task;
	int argc = 0;
	char *argv[20];
	while (*symbol == ' ') {
		symbol++;
	}
	if (*(argv[argc++] = symbol) != '\0') {
		while (*symbol != '\0') {
			if (*symbol == ' ') {
				*symbol = '\0';
				symbol++;
				while (*symbol == ' ') {
					symbol++;
				}
				if (*symbol != '\0') {
					argv[argc++] = symbol;
				}
				continue;
			}
			symbol++;
		}
		argv[argc] = NULL;

		for (int i = 0; i < ARRAY_SIZE(app_list); ++i) {
			if (!my_strcmp(argv[0], app_list[i].name)) {
				int child = os_sys_fork();
				if (-1 == child) {
					perror("fork");
				}
				int status = 0;
				const int waitflags = 0;
				if (child == 0) {
					if (in_fd != 0) {
						if (-1 == os_sys_dup2(in_fd, 0)) {
							perror("dup2");
						}
						if (-1 == os_sys_close(in_fd)) {
							perror("close");
						}
					}
					if (out_fd != 1) {
						if (-1 == os_sys_dup2(out_fd, 1)) {
							perror("dup2");
						}
						if (-1 == os_sys_close(out_fd)) {
							perror("dup2");
						}
					}
					if (-1 == os_sys_execv(app_list[i].path, argv)) {
						perror("execv");
					}
				}
				if (-1 == os_sys_waitpid(child, &status, waitflags)) {
					perror("waitpid");
				}
				return 0;
			}
		}
		if (-1 == os_sys_write(SHELL_WRONG_INPUT)) {
			perror("write");
		}
	}
	return 0;
}
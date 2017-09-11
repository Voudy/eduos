
#include "os.h"
#include "string.h"

void app1(void) {
	const char *msg = "Hello, World!\n";
	os_sys_write(msg);
}

void homework1(int argc, char *argv[]) {
	char *prefix = "Hello, ";
	char *user = "World";
	if (argc > 1) {
		user = argv[1];
	}
	char msg[256] = "";
	strcat(msg, prefix);
	strcat(msg, user);
	strcat(msg, "!\n");
	os_sys_write(msg);
}

int doTask(char *command) {
	char *splitArgs = strtok(command, " ");
	char *argv[256];
	int argc = 0;
	while (splitArgs) {
		argv[argc++] = splitArgs;
		splitArgs = strtok(NULL, " ");
	}
	if (strcmp(argv[0], "app1") == 0) {
		app1();
		return 1;
	} else if (strcmp(argv[0], "homework1") == 0) {
		homework1(argc, argv);
		return 1;
	} else if (strcmp(argv[0], "exit") == 0) {
		return 0;
	} else {
		char msg[256] = "No such function: ";
		strcat(msg, argv[0]);
		strcat(msg, "\n");
		os_sys_write(msg);
		return 1;
	}
}

void shell() {
	const char *welcomeMessage = "Welcome to eduos!\nAvailable commands: app1, homework1, exit\n";
	os_sys_write(welcomeMessage);
	int result = 1;
	while (result == 1) {
		os_sys_write("> ");	
		char buffer[256] = "";
		int readWord = os_sys_read(buffer, 256);
		buffer[strlen(buffer) - 1] = ';';
		char *splitCommand = strtok(buffer, ";");
		char *com[256];
		int comCnt = 0;
		while (splitCommand) {
			com[comCnt++] = splitCommand;
			splitCommand = strtok(NULL, ";");
		}
		for (int i = 0; i < comCnt; ++i) {
			result = doTask(com[i]);	
		}
	}
}

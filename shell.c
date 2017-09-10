
#include "os.h"
#include "string.h"

#define TRUE 1
#define FALSE 0

#define SHELL_INTRO "Welcome to shell!\nUse quit or q to quit from shell.\n"
#define SHELL_OUTRO "Good bye!\n"
#define SHELL_WRONG_INPUT "Application or command not found.\n"

int shell(void) {
    os_sys_write(SHELL_INTRO);
	while (TRUE) {
		const int string_size = 255;
		char buf[string_size];
		int actual_size = os_sys_read(buf, string_size * sizeof(char));
		buf[actual_size - 1] = ' ';
		buf[actual_size] = '\0'; 

		char *calls[30];
		int calls_amount = 0;
		char *call = strtok(buf, ";");
		while (call != NULL) {
			calls[calls_amount++] = call;
			call = strtok(NULL, ";");
		}

		for (int i = 0; i < calls_amount; i++) {
			char *words[30];
			int words_amount = 0;
			char* word = strtok(calls[i], " ");
			while (word != NULL) {
				words[words_amount++] = word;
				word = strtok(NULL, " ");
			}

			if (strcmp(words[0], "app1") == 0) {
                if (words_amount > 2) {
                    os_sys_write("Too many arguments!\n");
                }
                else {
                    app1(words_amount, words);
                }
            }
            else if (strcmp(words[0], "quit") == 0 || strcmp(words[0], "q") == 0) {
                os_sys_write(SHELL_OUTRO);
                return 0;
            }
            else {
                os_sys_write(SHELL_WRONG_INPUT);
            }
		}
	}

	return 0;
}
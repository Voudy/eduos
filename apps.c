
#include "os.h"
#include "string.h"

#define true 1
#define false 0

int app1(int argc, char *argv[]) {
	if (argc > 1) {
		os_sys_write("Hello, ");
		os_sys_write(argv[1]);
		os_sys_write("!\n");
	}
	else {
		os_sys_write("Hello, World!\n");
	}

	return 0;
}

int shell(void) {
	while (true) {
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
				app1(words_amount, words);
			}
		}
	}

	return 0;
}
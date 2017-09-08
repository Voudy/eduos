
#include "os.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

void printMsg(void) {
	const char *msg = "Hello, World!\n";
	os_sys_write(msg);
}

void readAndPrint(void) {
	const int maxWordLen = 255;
	size_t sizeOfWord = maxWordLen * sizeof(char);
	char *buffer = (char *) malloc(sizeOfWord);
	const char *readedWord = (const char *) (intptr_t) os_sys_read(buffer, sizeOfWord);
	os_sys_write(readedWord);
	free(buffer);
}

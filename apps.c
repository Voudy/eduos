
#include "os.h"

int app1(int argc, char* argv[]) {
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
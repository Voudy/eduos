
#include "os.h"

void app1(void) {
	const char *msg = "Hello, World!\n";
	os_sys_write(msg);
}

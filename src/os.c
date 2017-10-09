
#include <stddef.h>
<<<<<<< HEAD
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <ucontext.h>
#include <sys/ucontext.h>
#include <sys/queue.h>

#include "os.h"
#include "apps.h"

#define SYSCALL_X(x) \
	x(write) \
	x(read) \


#define ENUM_LIST(name) os_syscall_nr_ ## name,
enum syscalls_num {
	SYSCALL_X(ENUM_LIST)
};
#undef ENUM_LIST

typedef long(*sys_call_t)(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest);

static long errwrap(long res) {
	return res == -1 ? -errno : res;
}

static long sys_write(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest) {
	const char *msg = (const char *) arg1;
	return errwrap(write(STDOUT_FILENO, msg, strlen(msg)));
}

static int block_sig(int sig) {
	sigset_t newmask, oldmask;
	sigemptyset(&newmask);
	sigaddset(&newmask, sig);

	if (-1 == sigprocmask(SIG_BLOCK, &newmask, &oldmask)) {
		perror("block_sig: sigprocmask");
		exit(1);
	}

	return sigismember(&newmask, sig) ? sig : 0;
}

static void unblock_sig(int sig) {
	if (!sig) {
		return;
	}

	sigset_t newmask;
	sigemptyset(&newmask);
	sigaddset(&newmask, sig);

	if (-1 == sigprocmask(SIG_UNBLOCK, &newmask, NULL)) {
		perror("block_sig: sigprocmask");
		exit(1);
	}
}

enum sched_state {
	SCHED_FINISH,
	SCHED_READY,
	SCHED_SLEEP,
	SCHED_RUN,
};

struct sched_task {
	enum sched_state state;
	syshandler_t hnd;
	void *arg;
	int res;
	TAILQ_ENTRY(sched_task) link;
};

static struct sched_queue {
	struct sched_task tasks[256];
	TAILQ_HEAD(listhead, sched_task) head;
} schedule;

void sched_init() {
	TAILQ_INIT(&schedule.head);
}

void sched_add(enum sched_state state, int res, syshandler_t hnd, void *arg) {
	for (int i = 0; i < 256; ++i) {
		if (schedule.tasks[i].state == SCHED_FINISH) {
			struct sched_task *new_task = &schedule.tasks[i];
			new_task->state = state;
			new_task->res = res;
			new_task->hnd = hnd;
			new_task->arg = arg;
			TAILQ_INSERT_TAIL(&schedule.head, new_task, link);
			return;
		}
	}
}

void sched_notify(int res) {
	struct sched_task *task;
	TAILQ_FOREACH(task, &schedule.head, link) {
		if (task->state == SCHED_SLEEP) {
			task->state = SCHED_READY;
			task->res = res;
			return;
		}
	}
}

void sched_loop(void) {
	while (1) {
		if (TAILQ_EMPTY(&schedule.head)) {
			abort();
		}
		char is_found = 0;
		struct sched_task *task;
		TAILQ_FOREACH(task, &schedule.head, link) {
			if (task->state == SCHED_READY) {
				task->hnd(task->res, task->arg);
				task->state = SCHED_FINISH;
				TAILQ_REMOVE(&schedule.head, task, link);
				is_found = 1;
				break;
			}
		}
		if (!is_found) {
			pause();
		}
	}
}

static struct {
	void *buffer;
	int size;
} g_posted_read;

static long sys_read(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest) {
	void *buffer = (void *) arg1;
	const int size = (int) arg2;
	const syshandler_t hnd = (syshandler_t) arg3;
	void *const arg = (void *) arg4;

=======

#include "os.h"
#include "os/syscall.h"
#include "os/irq.h"
#include "os/sched.h"
#include "os/time.h"
>>>>>>> e98c7b13231733eef1409476fa950eb9774a3b03

#include "apps.h"

int main(int argc, char *argv[]) {
	irq_init();

	syscall_init();

	time_init();

	sched_init();

	sched_add(shell, NULL);

<<<<<<< HEAD
int main(int argc, char *argv[]) {
	os_init();
	sched_init();
	shell();
=======
>>>>>>> e98c7b13231733eef1409476fa950eb9774a3b03
	sched_loop();

	return 0;
}

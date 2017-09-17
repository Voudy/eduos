
#include <unistd.h>

#include "os.h"
#include "os/sched.h"

static struct {
	struct sched_task tasks[97];
	TAILQ_HEAD(listhead, sched_task) head;
} sched_task_queue;

void sched_add(enum sched_state state, int res, syshandler_t hnd, void *arg) {
	for (int i = 0; i < 97; ++i) {
		if (sched_task_queue.tasks[i].state == SCHED_FINISH) {
			struct sched_task *new_task = &sched_task_queue.tasks[i];
			new_task->state = state;
			new_task->res = res;
			new_task->hnd = hnd;
			new_task->arg = arg;
			TAILQ_INSERT_TAIL(&sched_task_queue.head, new_task, link);
			return;
		}
	}
}

void sched_notify(int res) {
	struct sched_task *task;
	TAILQ_FOREACH(task, &sched_task_queue.head, link) {
		if (task->state == SCHED_SLEEP) {
			task->state = SCHED_READY;
			task->res = res;
			return;
		}
	}
}

void sched_init(void) {
	TAILQ_INIT(&sched_task_queue.head);
}

void sched_loop(void) {
	while (1) {
		if (TAILQ_EMPTY(&sched_task_queue.head)) {
			return;
		}
		int flag = 0;
		struct sched_task *task;
		struct sched_task *next_task;
		TAILQ_FOREACH_SAFE(task, &sched_task_queue.head, link, next_task) {
			if (task->state == SCHED_READY) {
				task->state = SCHED_FINISH;
				TAILQ_REMOVE(&sched_task_queue.head, task, link);
				task->hnd(task->res, task->arg);
				flag = 1;
			}
		}
		if (!flag) {
			pause();
		}
	}
}




#include <unistd.h>
#include <stdlib.h>
#include "util.h"

#include "os.h"
#include "os/sched.h"

static struct {
	struct sched_task tasks[256];
	TAILQ_HEAD(listhead, sched_task) head;
	struct sched_task *current;
} sched_task_queue;

static struct sched_task *new_task(void) {
	for (int i = 0; i < ARRAY_SIZE(sched_task_queue.tasks); ++i) {
		if (sched_task_queue.tasks[i].state == SCHED_FINISH) {
			return &sched_task_queue.tasks[i];
		}
	}
	return NULL;
}

struct sched_task *sched_add(sched_task_entry_t entry, void *arg) {
	struct sched_task *task = new_task();

	if (!task) {
		abort();
	}

	task->state = SCHED_READY;
	task->entry = entry;
	task->arg = arg;
	TAILQ_INSERT_TAIL(&sched_task_queue.head, task, link);
	return task;
}

void sched_notify(struct sched_task *task) {
	task->state = SCHED_READY;
}

void sched_wait(void) {
	sched_current()->state = SCHED_SLEEP;
}

struct sched_task *sched_current(void) {
	return sched_task_queue.current;
}

void sched(void) {
	/* TODO context_switch */
	sched_current()->state = SCHED_RUN;
}

void sched_init(void) {
	TAILQ_INIT(&sched_task_queue.head);
}

static struct sched_task *next_task(void) {
	struct sched_task *task;
	TAILQ_FOREACH(task, &sched_task_queue.head, link) {
		if (task->state == SCHED_READY) {
			return task;
		}
	}
	return NULL;
}

void sched_loop(void) {
	while (1) {
		struct sched_task *task = next_task();
		if (task) {
			TAILQ_REMOVE(&sched_task_queue.head, task, link);
			sched_task_queue.current = task;
			task->state = SCHED_RUN;
			task->entry(task->arg);
			task->state = SCHED_FINISH;
		} else {
			pause();
		}
	}
}



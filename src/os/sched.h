
#ifndef EDUOS_OS_SCHED_H
#define EDUOS_OS_SCHED_H

#include "third-party/queue.h"

enum sched_state {
	SCHED_FINISH,
	SCHED_READY,
	SCHED_SLEEP,
	SCHED_RUN,
};

typedef void (*sched_task_t)(void *arg);

struct sched_task {
	TAILQ_ENTRY(sched_task) link;
	enum sched_state state;
	sched_task_t entry;
	void *arg;
};

extern void sched_add(sched_task_t entry, void *arg);
extern void sched_wait(void);
extern void sched_notify();
extern void sched(void);

extern void sched_init(void);
extern void sched_loop(void);

#endif /* EDUOS_OS_SCHED_H */


#define _GNU_SOURCE

#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "os.h"
#include "os/irq.h"

void (*irq_hnd)(void);
static sigset_t allmask;

static void os_sigiohnd(int sig, siginfo_t *info, void *ctx) {
	if (irq_hnd) {
		irq_hnd();
	}
}

static irqmask_t set2mask(sigset_t *set) {
	irqmask_t mask = 0;
	for (int i = 0; i < IRQ_MAX_SHIFT; ++i) {
		if (sigismember(set, i)) {
			mask |= 1 << i;
		}
	}
	return mask;
}

static void mask2set(irqmask_t mask, sigset_t *set) {
	sigemptyset(set);
	for (int i = 0; i < IRQ_MAX_SHIFT; ++i) {
		if (mask & (1 << i)) {
			sigaddset(set, i);
		}
	}
}

irqmask_t irq_disable(void) {
	sigset_t old;

	if (-1 == sigprocmask(SIG_BLOCK, &allmask, &old)) {
		perror("block_sig: sigprocmask");
		exit(1);
	}

	return set2mask(&old);
}

void irq_enable(irqmask_t mask) {
	if (!mask) {
		return;
	}

	sigset_t new;
	mask2set(mask, &new);
	if (-1 == sigprocmask(SIG_UNBLOCK, &new, NULL)) {
		perror("block_sig: sigprocmask");
		exit(1);
	}
}

int irq_init(void) {
	struct sigaction actio = {
		.sa_sigaction = os_sigiohnd,
		.sa_flags = SA_RESTART,
	};

	sigfillset(&allmask);

	sigemptyset(&actio.sa_mask);
	if (-1 == sigaction(SIGIO, &actio, NULL)) {
		perror("SIGIO set failed");
		exit(1);
	}

	if (-1 == fcntl(STDIN_FILENO, F_SETOWN, getpid())) {
		perror("fcntl SETOWN");
		exit(1);
	}

	int flags;
	if (-1 == (flags = fcntl(STDIN_FILENO, F_GETFL))) {
		perror("fcntl GETFL");
		exit(1);
	}

	flags |= O_NONBLOCK | O_ASYNC;
	if (-1 == fcntl(STDIN_FILENO, F_SETFL, flags)) {
		perror("fcntl SETFL");
		exit(1);
	}

	return 0;
}


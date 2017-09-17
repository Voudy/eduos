
#define _GNU_SOURCE

#define SYS_WRITE 0
#define SYS_READ 1
#define SYS_FORK 2
#define SYS_EXECV 3
#define SYS_DUP2 4
#define SYS_CLOSE 5
#define SYS_WAITPID 6
#define SYS_PIPE 7

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <sys/ucontext.h>

#include "os.h"

typedef long(*sys_call_t)(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest);

static long sys_write(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest) {
	const char *msg = (const char *) arg1;
	return write(STDOUT_FILENO, msg, strlen(msg));
}
static long sys_read(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest) {
	void *buffer = (void *) arg1;
	const int size = (int) arg2;
	return read(STDIN_FILENO, buffer, size);
}
static long sys_fork(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest) {
	return fork();
}
static long sys_execv(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest) {
	const char *path = (char*) arg1;
	char **argv = (char**) arg2;
	return execv(path, argv);
}
static long sys_dup2(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest) {
	int oldfd = (int) arg1;
	int newfd = (int) arg2;
	return dup2(oldfd, newfd);
}
static long sys_close(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest) {
	int fd = (int) arg1;
	return close(fd);
}
static long sys_waitpid(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest) {
	int pid = (int) arg1;
	int *status = (int*) arg2;
	int options = (int) arg3;
	return waitpid(pid, status, options);
}
static long sys_pipe(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest) {
	int *pipefd = (int*) arg1;
	return pipe(pipefd);
}

static const sys_call_t sys_table[] = {
	sys_write,
	sys_read,
	sys_fork,
	sys_execv,
	sys_dup2,
	sys_close,
	sys_waitpid,
	sys_pipe,
};

static void os_sighnd(int sig, siginfo_t *info, void *ctx) {
	ucontext_t *uc = (ucontext_t *) ctx;
	greg_t *regs = uc->uc_mcontext.gregs;

	if (0x81cd == *(uint16_t *) regs[REG_RIP]) {
		int ret = sys_table[regs[REG_RAX]](regs[REG_RAX], 
				regs[REG_RBX], regs[REG_RCX], 
				regs[REG_RDX], regs[REG_RSI], 
				(void *) regs[REG_RDI]);
		regs[REG_RAX] = ret;
		regs[REG_RIP] += 2;
	}
}

static void os_init(void) {
	struct sigaction act = {
		.sa_sigaction = os_sighnd,
		.sa_flags = SA_RESTART,
	};
	sigemptyset(&act.sa_mask);

	if (-1 == sigaction(SIGSEGV, &act, NULL)) {
		perror("signal set failed");
		exit(1);
	}
}

static long os_syscall(int syscall,
		unsigned long arg1, unsigned long arg2,
		unsigned long arg3, unsigned long arg4,
		void *rest) {
	long ret;
	__asm__ __volatile__(
		"int $0x81\n"
		: "=a"(ret)
		: "a"(syscall), "b"(arg1), "c"(arg2), "d"(arg3), "S"(arg4), "D"(rest)
		:
	);
	return ret;
}

int os_sys_write(const char *msg) {
	return os_syscall(SYS_WRITE, (unsigned long) msg, 0, 0, 0, NULL);
}
int os_sys_read(char *buffer, int size) {
	return os_syscall(SYS_READ, (unsigned long) buffer, size, 0, 0, NULL);
}
int os_sys_fork() {
	return os_syscall(SYS_FORK, 0, 0, 0, 0, NULL);
}
int os_sys_execv(const char *path, char *const argv[]) {
	return os_syscall(SYS_EXECV, (unsigned long) path, (unsigned long) argv, 0, 0, NULL);
}
int os_sys_dup2(int oldfd, int newfd) {
	return os_syscall(SYS_DUP2, (unsigned long) oldfd, (unsigned long) newfd, 0, 0, NULL);
}
int os_sys_close(int fd) {
	return os_syscall(SYS_CLOSE, (unsigned long) fd, 0, 0, 0, NULL);
}
int os_sys_waitpid(int pid, int *status, int options) {
	return os_syscall(SYS_WAITPID, (unsigned long) pid, (unsigned long) status, (unsigned long) options, 0, NULL);
}
int os_sys_pipe(int *pipefd) {
	return os_syscall(SYS_PIPE, (unsigned long) pipefd, 0, 0, 0, NULL);
}

int main(int argc, char *argv[]) {
	os_init();
	shell();
	return 0;
}
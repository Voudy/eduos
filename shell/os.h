#ifndef EDUOS_OS_H
#define EDUOS_OS_H

extern int os_sys_write(const char *msg);
extern int os_sys_read(char *buffer, int size);
extern int os_sys_fork();
extern int os_sys_execv(const char *path, char *const argv[]);
extern int os_sys_dup2(int oldfd, int newfd);
extern int os_sys_close(int fd);
extern int os_sys_waitpid(int pid, int *status, int options);
extern int os_sys_pipe(int *pipefd);

#endif /* EDUOS_OS_H */

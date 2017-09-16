
all : shell

clean :
	rm -f shell 1.out

test :
	../test/run.sh host_shell

CC = gcc
CFLAGS = -std=c99 -Wall -Werror

shell : ../src/sys_calls.o shell.o
	$(CC) $(LDFLAGS) -o $@ $^
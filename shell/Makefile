all : shell

CC = gcc
CFLAGS = -std=c99 -Wall -Werror

shell : shell.o
	$(CC) $(LDFLAGS) -o $@ $^

clean :
	rm -f shell 1.out

test :
	../test/run.sh host_shell

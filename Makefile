all : image

#src/apps.o : CFLAGS = -ffreestanding --sysroot=/tmp -Wimplicit-function-declaration -Werror

CC = gcc
CFLAGS = -Wall

image : src/os.o src/apps.o shell/shell.o shell/my_string.o
	$(CC) $(LDFLAGS) -o $@ $^

clean :
	rm -f image
	find -name '*.o' -delete

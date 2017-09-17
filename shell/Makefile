
shell : my_string.o shell.o 
	$(CC) $(LDFLAGS) -o $@ $^

clean :
	rm -f shell 1.out *.o

test :
	../test/run.sh host_shell

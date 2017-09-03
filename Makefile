
image : os.o apps.o
	$(CC) -o $@ $^
clean :
	rm -f image *.o

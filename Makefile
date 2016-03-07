CFLAGS=-Wall


project:(OBJS)
	gcc $(LDFLAGS) $< -o $@

%.o: %.c
	gcc $(CFLAGS) $< -c


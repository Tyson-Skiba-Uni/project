CFLAGS=-Wall
OBJS=project.o

project:$(OBJS)
	gcc $(LDFLAGS) $< -o $@

%.o: %.c
	gcc $(CFLAGS) $< -c


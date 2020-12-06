CC = gcc
TARGET = out
OBJS = main.o rd.o dokill.o

CFLAGS = -Wall -g -std=gnu11
LDFLAGS = -lc -g -std=gnu11
LDLIBS = -lcurses

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

.c.o :
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJS) $(TARGET)

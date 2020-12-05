CC = gcc
TARGET = out
OBJS = main.o rd.o dokill.o

CFLAGS = -Wall
LDFLAGS = -lc 
LDLIBS = -lcurses

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

.c.o :
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJS) $(TARGET)

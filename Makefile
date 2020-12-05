CC = gcc
TARGET = out
OBJS = main.o rd.o dokill.o

CFLAGS = -Wall
LDFLAGS = -lc

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS)  -o $@ $^

.c.o :
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJS) $(TARGET)

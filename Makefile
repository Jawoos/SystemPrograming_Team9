CC = gcc
TARGET = out
OBJS = main.o rd.o dokill.o

CFLAGS = -Wall
LDFLAGS = -lc 
AA = -lcurses

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(AA)

.c.o :
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJS) $(TARGET)

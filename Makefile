CC = gcc
TARGET = out
OBJS = main.o rd.o dokill.o

CFLAGS = -Wall
<<<<<<< HEAD
LDFLAGS = -lc
=======
LDFLAGS = -lc 
AA = -lcurses
>>>>>>> 82e5633624e96d7c72f8635151be8efef02f07f1

all : $(TARGET)

$(TARGET) : $(OBJS)
<<<<<<< HEAD
	$(CC) $(LDFLAGS)  -o $@ $^
=======
	$(CC) $(LDFLAGS) -o $@ $^ $(AA)
>>>>>>> 82e5633624e96d7c72f8635151be8efef02f07f1

.c.o :
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJS) $(TARGET)

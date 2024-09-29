CC = gcc
CFLAGS = -g
RM = rm -f
VAL = valgrind --tool=memcheck --log-file=memcheck.txt --leak-check=full --verbose

SRCS = main.c
OBJS = $(SRCS:.c=.o)

TARGET = a5

# default: all
testmemory: $(TARGET)
	$(VAL) ./$(TARGET)

all: a5

a5: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

clean:
	$(RM) $(TARGET) $(OBJS) memcheck.txt
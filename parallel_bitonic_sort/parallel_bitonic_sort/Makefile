CC=gcc
FLAGS=-std=c99 -lOpenCL
TARGET=main
SRCS=main.c
HEADS=
OBJS=$(subst .c,.o,$(SRCS))
RM=rm -f

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADS) $(OBJS)
	$(CC) $(FLAGS) -o $(TARGET) $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)

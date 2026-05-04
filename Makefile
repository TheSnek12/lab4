CC = gcc
CFLAGS = -g -Wall -Werror -std=gnu99

TARGET = p3_dbazler_007

all: $(TARGET)

$(TARGET): p3_satq.o
	$(CC) $(CFLAGS) $^ -o $@

p3_satq.o: p3_satq.c structs.h constants.h
	$(CC) $(CFLAGS) -c p3_satq.c -o p3_satq.o

clean:
	rm -f $(TARGET) *.o

#David Bazler G01582055
#CS 262 Lab Section 007
#Lab 10

CC = gcc
CFLAGS = -g -Wall -std=c99 -pedantic-errors
TARGET = lab10_dbazler_007
LIBS = -lm

$(TARGET) : $(TARGET).c
	$(CC) $(TARGET).c -o $(TARGET) $(CFLAGS) $(LIBS)

clean: 
	rm $(TARGET)
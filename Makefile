#David Bazler G01582055
#CS 262 Lab Section 007
#Project 2

CC = gcc
CFLAGS = -g -Wall -std=c99 -pedantic-errors
TARGET = p2_dbazler_007
LIBS = -lm

$(TARGET) : $(TARGET).c
	$(CC) $(TARGET).c -o $(TARGET) $(CFLAGS) $(LIBS)

clean: 
	rm $(TARGET)
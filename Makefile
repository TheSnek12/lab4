#David Bazler G01582055
#CS 262 Lab Section 007
#Project 1

CC = gcc
CFLAGS = -g -Wall -std=c99 -pedantic-errors
TARGET = p1_dbazler_007
LIBS = -lm

$(TARGET) : $(TARGET).c
	$(CC) $(TARGET).c -o $(TARGET) $(CFLAGS) $(LIBS)

clean: 
	rm $(TARGET)
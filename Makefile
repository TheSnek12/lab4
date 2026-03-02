#David Bazler G01582055
#CS 262 Lab Section 007
#Lab 4

CC = gcc
CFLAGS = -g -Wall -std=c99 -pedantic-errors
TARGET = lab4_dbazler_007
LIBS = -lm

$(TARGET) : $(TARGET).c
	$(CC) $(TARGET).c -o $(TARGET) $(CFLAGS) $(LIBS)

clean: 
	rm $(TARGET)
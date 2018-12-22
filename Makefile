# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall
CCLINK = $(CC)
OBJS =  main.o ATM.o Bank.o Account.o
RM = rm -f
# Creating the  executable
Bank: $(OBJS)
	$(CCLINK) -o Bank $(OBJS) -pthread
# Creating the object files
Account.o: Account.cpp Account.h
ain.o: main.cpp Bank.h ATM.h Account.h
Bank.o: Bank.cpp Bank.h
ATM.o: ATM.cpp ATM.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

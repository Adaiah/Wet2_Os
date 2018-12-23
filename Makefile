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
main.o: main.cpp Bank.h ATM.h Account.h
Bank.o: Bank.cpp Bank.h Account.h
ATM.o: ATM.cpp ATM.h Account.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

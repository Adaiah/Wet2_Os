# Makefile for the smash program
CC = g++
CFLAGS = -std=c++11 -g -Wall
CCLINK = $(CC)
OBJS = main.o bank.o atm.o account.o
RM = rm -f
# Creating the  executable
Bank: $(OBJS)
	$(CCLINK) -o Bank $(OBJS)
# Creating the object files
account.o: Account.cpp Account.h
atm.o: ATM.cpp ATM.h
bank.o: Bank.cpp Bank.h
main.o: main.cpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

//
// Created by Adaiah on 15-Dec-18.
//

#include "ATM.h"
 void *miniMainATM(void* args);

 bool ATMAction(void* inputFile, void* logFile);
 bool openAccount(int accountId, int password, int initial_amount);
 bool makeAccountVIP(int accountId, int password);
 bool Deposit(int accountId, int password, int amount);
 bool withdraw(int accountId, int password, int initial_amount);
 bool checkAmount(int accountId, int password);
 bool Transfer(int src_accountId, int src_password,int dest_accountId, int dest_password, int initial_amount);


//
// Created by Adaiah on 15-Dec-18.
//

#include "ATM.h"
pthread_t write_lock;

 void *miniMainATM(void* args);

 bool ATMAction(void* inputFile, void* logFile);
 bool openAccount(int accountId, int password, int initial_amount);
 bool Deposit(int accountId, int password, int amount);
 bool checkAmount(int accountId, int password);

=======


bool isAccountExist(int accountID);

void makeVIP(int AtmID, int accountID, unsigned short int password) {
    if (!isAccountExist(accountID)) {
        cout << "Error " << AtmID << ": Your transaction failed - account id " << accountID
             << " does not exist" << endl;
        return;
    }
    else if (!bank_accounts[accountID].checkPassword(password)) {
        cout << "Error " << AtmID << ": Your transaction failed - password for account id " << accountID
             << " is incorrect" << endl;
        return;
    }
    bank_accounts[accountID].setAccVIP();
}

void withdraw(int AtmID, int accountID, unsigned short int password, unsigned int amount){
    if (!isAccountExist(accountID)) {
        cout << "Error " << AtmID << ": Your transaction failed - account id " << accountID
             << " does not exist" << endl;
        return;
    }
    else if (!bank_accounts[accountID].checkPassword(password)) {
        cout << "Error " << AtmID << ": Your transaction failed - password for account id " << accountID
             << " is incorrect" << endl;
        return;
    }

    int new_balance = bank_accounts[accountID].setBalance(MINUS, amount))
    if(new_balance < 0){  //couldn't withdraw from the account
        cout << "Error " << AtmID << ": Your transaction failed - account id " << accountID << " balance is lower than "
            << amount << endl;
        return;
    } else{
        cout << AtmID << ": Account "<< accountID << " new balance is " << new_balance << " after " << amount
            << " $ was withdrew" << endl;
        return;
    }
}

void transfer(int AtmID, int fromAccID, unsigned short int password, int toAccId, unsigned int amount){
    if (!isAccountExist(fromAccID)) {
        cout << "Error " << AtmID << ": Your transaction failed - account id " << fromAccID
             << " does not exist" << endl;
        return;
    }
    else if (!bank_accounts[fromAccID].checkPassword(password)) {
        cout << "Error " << AtmID << ": Your transaction failed - password for account id " <<  fromAccID
             << " is incorrect" << endl;
        return;
    }
    if (!isAccountExist(toAccID)) {
        cout << "Error " << AtmID << ": Your transaction failed - account id " << toAccId
             << " does not exist" << endl;
        return;
    }

    int new_source_balance = bank_accounts[fromAccID].setBalance(MINUS, amount))
    if(new_source_balance < 0) {  //couldn't withdraw from the account
        cout << "Error " << AtmID << ": Your transaction failed - account id " << fromAccID << " balance is lower than "
             << amount << endl;
        return;
    }
    int new_dest_balance = bank_accounts[toAccId].setBalance(PLUS, amount))
    if(new_dest_balance > 0) {  //couldn't withdraw from the account
        cout << AtmID << ": Transfer " << amount << " from account " << fromAccID << " to account " << toAccId
            << " new account " << endl << "balance is " << new_source_balance << "new target account balance is "
            << new_dest_balance << endl;
        return;
    }


}

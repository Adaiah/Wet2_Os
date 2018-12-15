//
// Created by Adaiah on 15-Dec-18.
//

#include "ATM.h"
pthread_mutex_t write_lock;

 void *miniMainATM(void* args){
    Atm_args* atm_args = (Atm_args*)args;
    int num_of_atm = atm_atgs->num_of_atm;
    int rc, i;
    pthread_t atms_t[num_of_atm];
    atm_input_output atmIO[num_of_atm];
    for(i = 0 ; i < num_of_atm ; ++i ){
        atmIO[i].log_file_name = atm_args->log_file_name;
        atmIO[i].input_file = atm_args->input_files[i];
         if(rc = pthread_create(&atms_t[i], NULL, ATMAction, &atmIO[i])){
             fprints(sderr, "error: pthread_create, rc: %d\n", rc);
             return NULL; //todo: check what to return
         }
    }
    for (i=0 , i < num_of_atm , ++i){
        pthread_join(atms_t[i], NULL);
    }

    return NULL; //todo : check what to return
 }

 void *ATMAction(void* args);
 bool openAccount(int accountId, int password, int initial_amount);
 bool Deposit(int accountId, int password, int amount);
 bool checkAmount(int accountId, int password);

=======


bool isAccountExist(int accountID);

void makeVIP(int AtmID, int accountID, unsigned short int password) {
    if (!isAccountExist(accountID)) {
        cout << "Error " << AtmID << ": Your transaction failed - account id " << accountID
             << " does not exist" << endl; //todo: change to write in file
        return;
    }
    else if (!bank_accounts[accountID].checkPassword(password)) {
        cout << "Error " << AtmID << ": Your transaction failed - password for account id " << accountID
             << " is incorrect" << endl; //todo: change to write in file
        return;
    }
    bank_accounts[accountID].setAccVIP();
}

void withdraw(int AtmID, int accountID, unsigned short int password, unsigned int amount){
    if (!isAccountExist(accountID)) {
        cout << "Error " << AtmID << ": Your transaction failed - account id " << accountID
             << " does not exist" << endl; //todo: change to write in file
        return;
    }
    else if (!bank_accounts[accountID].checkPassword(password)) {
        cout << "Error " << AtmID << ": Your transaction failed - password for account id " << accountID
             << " is incorrect" << endl; //todo: change to write in file
        return;
    }

    int new_balance = bank_accounts[accountID].setBalance(MINUS, amount))
    if(new_balance < 0){  //couldn't withdraw from the account
        cout << "Error " << AtmID << ": Your transaction failed - account id " << accountID << " balance is lower than "
            << amount << endl; //todo: change to write in file
        return;
    } else{
        cout << AtmID << ": Account "<< accountID << " new balance is " << new_balance << " after " << amount
            << " $ was withdrew" << endl; //todo: change to write in file
        return;
    }
}

void transfer(int AtmID, int fromAccID, unsigned short int password, int toAccId, unsigned int amount){
    if (!isAccountExist(fromAccID)) {
        cout << "Error " << AtmID << ": Your transaction failed - account id " << fromAccID
             << " does not exist" << endl; //todo: change to write in file
        return;
    }
    else if (!bank_accounts[fromAccID].checkPassword(password)) {
        cout << "Error " << AtmID << ": Your transaction failed - password for account id " <<  fromAccID
             << " is incorrect" << endl; //todo: change to write in file
        return;
    }
    if (!isAccountExist(toAccID)) {
        cout << "Error " << AtmID << ": Your transaction failed - account id " << toAccId
             << " does not exist" << endl;  //todo change to write in file
        return;
    }

    int new_source_balance = bank_accounts[fromAccID].setBalance(MINUS, amount))
    if(new_source_balance < 0) {  //couldn't withdraw from the account
        cout << "Error " << AtmID << ": Your transaction failed - account id " << fromAccID << " balance is lower than "
             << amount << endl;  //todo: chnage to write in file
        return;
    }
    int new_dest_balance = bank_accounts[toAccId].setBalance(PLUS, amount))
    if(new_dest_balance > 0) {  //couldn't withdraw from the account
        cout << AtmID << ": Transfer " << amount << " from account " << fromAccID << " to account " << toAccId
            << " new account " << endl << "balance is " << new_source_balance << "new target account balance is "
            << new_dest_balance << endl;  //todo: chnage to write in file
        return;
    }


}

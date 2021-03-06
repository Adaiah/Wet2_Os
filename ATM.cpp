//
// Created by Adaiah on 15-Dec-18.
//

#include "ATM.h"
pthread_mutex_t write_lock;

void openAccount(int AtmID, int accountID, int password, int initial_amount);
void makeAccountVIP(int AtmID, int accountID, unsigned short int password);
void Deposit(int AtmID, int accountID, int password, int amount);
void withdraw(int AtmID, int accountID, unsigned short int password, unsigned int amount);
void checkAmount(int AtmID, int accountId, int password);
bool isAccountExist(int accountID);
void transfer(int AtmID, int fromAccID, unsigned short int password, int toAccId, unsigned int amount);


//********************************************
// function name: ATMAction
// Description: retrieves command line and parameters from the input file and calls for the appropriate function to handle the action
// Parameters: receives the number of the ATM and the input file
// Returns: N/A
//**************************************************************************************
void *ATMAction(void* args){

    atm_input_output* atm_args= (atm_input_output*)args;
    ifstream inputFile(atm_args->input_file.c_str());
    if(!inputFile.is_open()){//check file successfully opened
        perror("Error ");
        pthread_exit(NULL);
    }
    int ATM_id=atm_args->ATM_ID;
    string curr_line;

    char action;
    int accountId, accountId_target;
    unsigned short int password;
    unsigned int balance, amount;

    while(getline(inputFile, curr_line)) {
        usleep(100000); //sleep for 100 milliseconds
        stringstream action_arg;
        action_arg.str(curr_line);
        action_arg >> action >> accountId >> password;
        if(action == 'O'){  //open new account
            action_arg >> amount;
            openAccount(ATM_id, accountId,password,amount);
        }
        else if (action =='L') { // make account VIP
            makeAccountVIP(ATM_id, accountId,password);
        }
        else if (action == 'D'){ //deposit
            action_arg >> amount;
            Deposit(ATM_id, accountId,password,amount);
        }
        else if (action == 'W') { //withdraw
            action_arg >> amount;
            withdraw(ATM_id, accountId, password,amount);
        }
        else if (action == 'B') { //show current balance
            checkAmount(ATM_id,accountId,password);
        }
        else if (action == 'T') { //transfer
            action_arg >> accountId_target >> amount;
            transfer(ATM_id, accountId, password, accountId_target, amount);
        }
    }
    inputFile.close();
    pthread_exit(NULL);
}


//********************************************
// function name: printStatus
// Description:
// Parameters:
// Returns:
//**************************************************************************************
 void *miniMainATM(void* args){ //
    Atm_args* atm_args = (Atm_args*)args;
    int tot_num_of_atm = atm_args->tot_num_of_atm;
    int rc, i;
    pthread_t atms_t[tot_num_of_atm];
    atm_input_output atmIO[tot_num_of_atm];
    for(i = 0 ; i < tot_num_of_atm ; ++i ){
        atmIO[i].input_file = atm_args->input_files[i];
        atmIO[i].ATM_ID = i+1; //todo make sure this is ok
         if(rc = pthread_create(&atms_t[i], NULL, ATMAction, &atmIO[i])){
             pthread_mutex_lock(&log_write_mut);
             logfile << "error: pthread_create, rc: "<< rc<<endl;
             pthread_mutex_unlock(&log_write_mut);
             pthread_exit(NULL);
         }
    }
    for (i=0 ; i < tot_num_of_atm ; ++i){
        pthread_join(atms_t[i], NULL);
    }
    finished_all_actions = true;

    pthread_exit(NULL);
 }


//********************************************
// function name: openAccount
// Description: creates a new account if there are no other accounts with the same account number
// Parameters: AtmID - number of current ATM
//             accountID - ID number of the current account
//             password
//             initial_amount - amount to enter to the new account
// Returns: N/A
//**************************************************************************************
 void openAccount(int AtmID, int accountID, int password, int initial_amount){
    pthread_mutex_lock(&bank_account_mut);
    sleep(1);
    if (isAccountExist(accountID)) { //account exists
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account with the same id exists" << endl;
        pthread_mutex_unlock(&log_write_mut);
        pthread_mutex_unlock(&bank_account_mut);
        return;
    }
    else { // create and enter the new account into the bank_accounts map for future use
        pthread_mutex_lock(&log_write_mut);
        Account new_Account(accountID , password, initial_amount);
        bank_accounts[accountID]=new_Account;
        logfile << AtmID << ": New account id is " << accountID << " with password " << password << " and initial balance " << initial_amount << endl;
        pthread_mutex_unlock(&log_write_mut);
        pthread_mutex_unlock(&bank_account_mut);

        return;
    }
 }

//********************************************
// function name: Deposit
// Description: adds an amount of money to the account
// Parameters: AtmID - number of current ATM
//             accountID - ID number of the current account
//             password
//             amount - amount to enter to the account
// Returns: N/A
//**************************************************************************************
void Deposit(int AtmID, int accountId, int password, int amount){
    pthread_mutex_lock(&bank_account_mut);
    if (!isAccountExist(accountId)) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << accountId
                << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);
        pthread_mutex_unlock(&bank_account_mut);
        return;
    }
    pthread_mutex_unlock(&bank_account_mut);

    if (!bank_accounts[accountId].checkPassword(password)) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - password for account id " << accountId
                << " is incorrect" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    bank_accounts[accountId].setBalance(DEPOSIT,amount, AtmID);
    return;
 }

//********************************************
// function name: checkAmount
// Description: checks the amount existing in an account
// Parameters: AtmID - number of current ATM
//             accountID - ID number of the current account
//             password
// Returns: N/A
//**************************************************************************************
void checkAmount(int AtmID, int accountId, int password) {
    int curr_balance=0;
    pthread_mutex_lock(&bank_account_mut);
    if (!isAccountExist(accountId)) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << accountId
                << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);
        pthread_mutex_unlock(&bank_account_mut);
        return;
    }
    pthread_mutex_unlock(&bank_account_mut);

    if (!bank_accounts[accountId].checkPassword(password)) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - password for account id " << accountId
                << " is incorrect" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    bank_accounts[accountId].printBalance(AtmID);
    return;

 }

//=======

//********************************************
// function name: isAccountExist
// Description: returns if the account exists in the map structure holding ATM accounts
// Parameters: accountID- the account number of interest
// Returns: true- exists, false - does not exists.
//**************************************************************************************
bool isAccountExist(int accountID){
    map<int, Account>::iterator it;

    it =bank_accounts.find(accountID);
    if (it == bank_accounts.end()) { // account not found
        return false;
    }
    else return true;


}

//********************************************
// function name: makeAccountVIP
// Description: make account VIP, bank can't take commission
// Parameters: AtmID - number of current ATM
//             accountID - ID number of the current account
//             password
// Returns: N/A
//**************************************************************************************
void makeAccountVIP(int AtmID, int accountID, unsigned short int password) {
    pthread_mutex_lock(&bank_account_mut);
    if (!isAccountExist(accountID)) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << accountID
             << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);
        pthread_mutex_unlock(&bank_account_mut);
        return;
    }
    pthread_mutex_unlock(&bank_account_mut);

    if (!bank_accounts[accountID].checkPassword(password)) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - password for account id " << accountID
             << " is incorrect" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    bank_accounts[accountID].setAccVIP();
}

//********************************************
// function name: withdraw
// Description: withdraw the entered amount form the account if legal
// Parameters: AtmID - number of current ATM
//             accountID - ID number of the current account
//             password
//             amount - amount to withdraw from the account
// Returns: N/A
//**************************************************************************************
void withdraw(int AtmID, int accountID, unsigned short int password, unsigned int amount){

    pthread_mutex_lock(&bank_account_mut);
    if (!isAccountExist(accountID)) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << accountID
             << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);
        pthread_mutex_unlock(&bank_account_mut);
        return;
    }
    pthread_mutex_unlock(&bank_account_mut);

    if (!bank_accounts[accountID].checkPassword(password)) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - password for account id " << accountID
             << " is incorrect" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }

    bank_accounts[accountID].setBalance(WITHDRAW,amount,AtmID);
    return;
}

//********************************************
// function name: transfer
// Description: transfers money from one account to another after checking parameters of the
//              action. if transfer is to the same account do nothing.
// Parameters: AtmID - number of current ATM
//             fromAccID - ID number of the account to take money from
//             password
//             toAccID - ID number of the account to deposit money to
//             amount - amount to transfer to the account
// Returns: N/A
//**************************************************************************************
void transfer(int AtmID, int fromAccID, unsigned short int password, int toAccId, unsigned int amount){

    pthread_mutex_lock(&bank_account_mut);
    if (!isAccountExist(fromAccID)) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << fromAccID
             << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);
        pthread_mutex_unlock(&bank_account_mut);
        return;
    }
    pthread_mutex_unlock(&bank_account_mut);

    if (!bank_accounts[fromAccID].checkPassword(password)) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - password for account id " <<  fromAccID
             << " is incorrect" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    if (fromAccID == toAccId) //if self transfer - do nothing
        return;

    pthread_mutex_lock(&bank_account_mut);
    if (!isAccountExist(toAccId)) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << toAccId
             << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);
        pthread_mutex_unlock(&bank_account_mut);
        return;
    }
    pthread_mutex_unlock(&bank_account_mut);


    //locking the balances for reading and writing for both of the accounts
    if(fromAccID>toAccId) { // lock the account with lower accountID first to avoid deadlock
        bank_accounts[toAccId].lockSetAccount();
        bank_accounts[fromAccID].lockSetAccount();
    }
    else {
        bank_accounts[fromAccID].lockSetAccount();
        bank_accounts[toAccId].lockSetAccount();

    }
    sleep(1);
    unsigned int curr_from_balance = bank_accounts[fromAccID].balance;

    if( (int)curr_from_balance - (int)amount < 0) {  //couldn't withdraw from the account
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << fromAccID << " balance is lower than "
             << amount << endl;
        pthread_mutex_unlock(&log_write_mut);
        bank_accounts[fromAccID].unlockSetAccount();
        bank_accounts[toAccId].unlockSetAccount();
        return;
    }

    bank_accounts[fromAccID].balance = curr_from_balance - amount;
    unsigned int new_source_balance = bank_accounts[fromAccID].balance;

    bank_accounts[toAccId].balance =  bank_accounts[toAccId].balance + amount;
    unsigned int new_dest_balance = bank_accounts[toAccId].balance;
    pthread_mutex_lock(&log_write_mut);
    logfile << AtmID << ": Transfer " << amount << " from account " << fromAccID << " to account " << toAccId
            << " new account " << "balance is " << new_source_balance << " new target account balance is "
            << new_dest_balance << endl;
    pthread_mutex_unlock(&log_write_mut);

    //unlocking the balances for reading and writing for both of the accounts
    // unlocking does not care for the position of id within the map
    bank_accounts[fromAccID].unlockSetAccount();
    bank_accounts[toAccId].unlockSetAccount();

    return;
}




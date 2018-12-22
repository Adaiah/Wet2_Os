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
    int ATM_id=atm_args->ATM_ID;
    string curr_line;

    char action;
    int accountId, accountId_target;
    unsigned short int password;
    unsigned int balance, amount;

    //inputFile.open(atm_args->input_file);
    while(getline(inputFile, curr_line)) {
        stringstream action_arg;
//        cout<<"debug: "<<curr_line<<endl ; //todo:debug
        action_arg.str(curr_line);
        action_arg >> action >> accountId >> password;
//        cout<<"Debug: "<<action<<endl; //todo:debug
        if(action == 'O'){
            action_arg >> amount;
            openAccount(ATM_id, accountId,password,amount);
        }
        else if (action =='L') {
            makeAccountVIP(ATM_id, accountId,password);
        }
        else if (action == 'D'){
            action_arg >> amount;
            Deposit(ATM_id, accountId,password,amount);
        }
        else if (action == 'W') {
            action_arg >> amount;
            withdraw(ATM_id, accountId, password,amount);
        }
        else if (action == 'B') {
            checkAmount(ATM_id,accountId,password);
        }
        else if (action == 'T') {
            action_arg >> accountId_target >> amount;
            transfer(ATM_id, accountId, password, accountId_target, amount);
        }
        usleep(100000);
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
             cout << "error: pthread_create, rc: "<< rc<<endl;
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
//    cout<<"Debugd: openAccout"<<endl; //todo:debug
     sleep(1);
     pthread_mutex_lock(&insert_new_account_mut);
     if (isAccountExist(accountID)) { //account exists
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account with the same id exists" << endl;
        pthread_mutex_unlock(&log_write_mut);
        pthread_mutex_unlock(&insert_new_account_mut);
        return;
    }
    else { // create and enter the new account into the bank_accounts map for future use
        Account new_Account(accountID , password, initial_amount);
        bank_accounts[accountID]=new_Account;
        pthread_mutex_unlock(&insert_new_account_mut);
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
//    cout<<"Debugd: Deposit"<<endl; //todo:debug
    sleep(1);
    if (!isAccountExist(accountId)) {
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << accountId
                << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    else if (!bank_accounts[accountId].checkPassword(password)) {
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - password for account id " << accountId
                << " is incorrect" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    bank_accounts[accountId].setBalance(true,amount,0);
    pthread_mutex_lock(&log_write_mut);
    logfile << AtmID << ": Account " << accountId << " new balance is " << bank_accounts[accountId].getBalance() << " after "
    << amount << " $ was deposited" << endl;
    pthread_mutex_unlock(&log_write_mut);
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
//    cout<<"Debugd: checkAmount"<<endl; //todo:debug
    sleep(1);
    if (!isAccountExist(accountId)) {
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << accountId
                << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    else if (!bank_accounts[accountId].checkPassword(password)) {
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - password for account id " << accountId
                << " is incorrect" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    pthread_mutex_lock(&log_write_mut);
    logfile << AtmID << ": Account " << accountId << " balance is " << bank_accounts[accountId].getBalance() << endl;
    pthread_mutex_unlock(&log_write_mut);
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

void makeAccountVIP(int AtmID, int accountID, unsigned short int password) {
//    cout<<"Debugd: makeAccountVIP"<<endl; //todo:debug
//TODO: changed to write to file, please add locks to the rest of your logging
    sleep(1);
    if (!isAccountExist(accountID)) {
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << accountID
             << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);

        return;
    }
    else if (!bank_accounts[accountID].checkPassword(password)) {
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - password for account id " << accountID
             << " is incorrect" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    bank_accounts[accountID].setAccVIP();
}

//********************************************
// function name: printStatus
// Description:
// Parameters:
// Returns:
//**************************************************************************************
void withdraw(int AtmID, int accountID, unsigned short int password, unsigned int amount){
//    cout<<"Debugd: Withdraw"<<endl; //todo:debug
    sleep(1);
    if (!isAccountExist(accountID)) {
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << accountID
             << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    else if (!bank_accounts[accountID].checkPassword(password)) {
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - password for account id " << accountID
             << " is incorrect" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }

    int new_balance = bank_accounts[accountID].setBalance(MINUS, amount,0);
    if(new_balance < 0){  //couldn't withdraw from the account
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << accountID << " balance is lower than "
            << amount << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    } else{
        pthread_mutex_lock(&log_write_mut);
        logfile << AtmID << ": Account "<< accountID << " new balance is " << new_balance << " after " << amount
            << " $ was withdrew" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
}

//********************************************
// function name: printStatus
// Description:
// Parameters:
// Returns:
//**************************************************************************************
void transfer(int AtmID, int fromAccID, unsigned short int password, int toAccId, unsigned int amount){
    sleep(1);
    if (!isAccountExist(fromAccID)) {
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << fromAccID
             << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    else if (!bank_accounts[fromAccID].checkPassword(password)) {
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - password for account id " <<  fromAccID
             << " is incorrect" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    if (fromAccID == toAccId) //if self transfer - do nothing
        return;
    if (!isAccountExist(toAccId)) {
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << toAccId
             << " does not exist" << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }

    int new_source_balance = bank_accounts[fromAccID].setBalance(MINUS, amount,0);
    if(new_source_balance < 0) {  //couldn't withdraw from the account
        pthread_mutex_lock(&log_write_mut);
        logfile << "Error " << AtmID << ": Your transaction failed - account id " << fromAccID << " balance is lower than "
             << amount << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }
    int new_dest_balance = bank_accounts[toAccId].setBalance(PLUS, amount,0);
    if(new_dest_balance > 0) {  //couldn't withdraw from the account
        pthread_mutex_lock(&log_write_mut);
        logfile << AtmID << ": Transfer " << amount << " from account " << fromAccID << " to account " << toAccId
            << " new account " << endl << "balance is " << new_source_balance << "new target account balance is "
            << new_dest_balance << endl;
        pthread_mutex_unlock(&log_write_mut);
        return;
    }


}

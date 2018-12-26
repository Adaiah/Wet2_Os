//
// Created by Adaiah on 14-Dec-18.
//
#include "Account.h"

//********************************************
// function name: Account
// Description: c'tr, locks are unlocked initially
// Parameters: AtmID - number of current ATM
//             accountID - ID number of the current account
//             password
//             balance - amount to enter to the new account
// Returns: N/A
//**************************************************************************************
Account::Account(int accountId , unsigned short int password  , int balance):accountId(accountId),
        password(password), balance(balance), isVIP(false), account_readcount(0), account_writecount(0),
        commission_taken(0){
    pthread_mutex_init(&account_read, NULL);
    pthread_mutex_init(&account_write, NULL);
    pthread_mutex_init(&account_readtry, NULL);
    pthread_mutex_init(&account_resource, NULL);

}

//********************************************
// function name: ~Account
// Description: d'tr, opens mutex if locked in order to destroy. avoid undefined behaviour
// Parameters: N/A
// Returns: N/A
//**************************************************************************************
Account::~Account(){
    if(!pthread_mutex_trylock(&account_read)) {
        pthread_mutex_unlock(&account_read);
    }
    else
        pthread_mutex_destroy(&account_read);

    if(!pthread_mutex_trylock(&account_write)) {
        pthread_mutex_unlock(&account_write);
    }
    else
        pthread_mutex_destroy(&account_write);


    if(!pthread_mutex_trylock(&account_readtry)) {
        pthread_mutex_unlock(&account_readtry);
    }
    else
        pthread_mutex_destroy(&account_readtry);


    if(!pthread_mutex_trylock(&account_resource)) {
        pthread_mutex_unlock(&account_resource);
    }
    else
        pthread_mutex_destroy(&account_resource);
}


//Methods to access data individually

//********************************************
// function name: printBalance
// Description: gets balance in the account
// Parameters: N/A
// Returns: N/A
//**************************************************************************************
void Account::printBalance(unsigned int atm_id){

    unsigned int curr_balance = 0 ;

    // reader lock
    this->lockGetAccount();

    curr_balance = this->balance;

    sleep(1);
    pthread_mutex_lock(&log_write_mut);
    logfile << atm_id << ": Account " << this->accountId << " balance is " << curr_balance << endl;
    pthread_mutex_unlock(&log_write_mut);

    // reader unlock
    this->unlockGetAccount();

    return;
}

//********************************************
// function name: getAccountId
// Description: return account ID, no need for protection
// Parameters: N/A
// Returns: N/A
//**************************************************************************************
int Account:: getAccountId(){

    return accountId;
}


//********************************************
// function name: checkPassword
// Description: checks if an entered password matches the one of the acccount
// Parameters: password - the entered password
// Returns: true- if matches, false- no match
//**************************************************************************************
bool Account::checkPassword(unsigned short int password) {
    if(this->password == password) return true;
    return false;
}

//Methods to change  the data

//********************************************
// function name: setAccVIP
// Description: turns the account to VIP
// Parameters: NONE
// Returns: NULL
//**************************************************************************************
void Account::setAccVIP(){

    this->lockSetAccount();

    this->isVIP=true;
    sleep(1);

    this->unlockSetAccount();
}


//********************************************
// function name: addCommission
// Description: add a commission to the commission_taken
// Parameters: int commission
// Returns: void
//**************************************************************************************
void Account::addCommission(int commission){
    this->commission_taken += commission;
}

//********************************************
// function name: takeCommission
// Description: used by the bank to take commission
// Parameters: double commission
// Returns: void
//**************************************************************************************
void Account::takeCommission( double commission_rate){
    if(!this->isVIP) { // if not a vip

        double commission_d = balance * (commission_rate / 100);

        int commission = commission_d;
        if (commission + 0.5 < commission_d)
            commission = commission + 1;

        balance = balance - commission;
        pthread_mutex_lock(&log_write_mut);
        logfile<<"Bank: commission of " << commission_rate << " % " << "were charged, the bank gained " << commission << " $ from account " << getAccountId() << endl;
        pthread_mutex_unlock(&log_write_mut);

        //add the commission taken to the place which thr bank can take in the printBank
        this->addCommission(commission);

    }
    return;

}

//********************************************
// function name: setBalance
// Description: set the new balance by adding or decreasing the amount
// Parameters: bool sign , unsigned int amount , int commission_rate
// Returns: int
//**************************************************************************************
int Account::setBalance(ATM_Action atm_action, int amount, unsigned int atm_id ) {
    int curr_balance = 0;

    // writers lock
    this->lockSetAccount();

    sleep(1);
    if((atm_action == WITHDRAW) ) {  //decrease
        if ((balance - amount ) < 0){ //if turn into to negative
            curr_balance = -1;
        } else{
            balance = balance - amount ;
            curr_balance = balance;
        }
    } else{  //DEPOSIT = increase
        balance = balance + amount;
        curr_balance = balance;
    }


    if (atm_action == DEPOSIT){
        pthread_mutex_lock(&log_write_mut);
        logfile << atm_id << ": Account " << this->accountId << " new balance is " << curr_balance << " after "
        << amount << " $ was deposited" << endl;
        pthread_mutex_unlock(&log_write_mut);
    }
    else if (atm_action == WITHDRAW){
        if (curr_balance <0){
            pthread_mutex_lock(&log_write_mut);
            logfile << "Error " << atm_id << ": Your transaction failed - account id " << this->accountId << " balance is lower than "
            << amount << endl;
            pthread_mutex_unlock(&log_write_mut);
        }
        else{
            pthread_mutex_lock(&log_write_mut);
            logfile << atm_id << ": Account "<< this->accountId << " new balance is " << curr_balance << " after " << amount
            << " $ was withdrew" << endl;
            pthread_mutex_unlock(&log_write_mut);
        }
    }

    // writers unlock
    this->unlockSetAccount();

    return curr_balance;
}
//********************************************
// function name: printAccount
// Description: prints the account main details (id, balance, password)
// Parameters: none
// Returns: none
//**************************************************************************************
void Account::printAccount(){
    // no need for lock because under bank lock
    cout <<"Account "<< this->accountId <<": Balance - " << this->balance
        << " $ , Account Password - " << this->password << endl;
    return ;
}



//********************************************
// function name: getCommissionTaken
// Description: get the commission which been taken from the account and zeroing it
// Parameters: none
// Returns: int
//**************************************************************************************
int Account::getCommissionTaken(){
    //no need for lock because under bank lock
    int commission = this->commission_taken;
    this->commission_taken = 0;
    return commission;
}


//********************************************
// function name: lockSetAccount
// Description: lock the account for writer
// Parameters: none
// Returns: none
//**************************************************************************************
void Account::lockSetAccount(){
    // concurrency between Bank lock (locks all the accounts for writing)
    pthread_mutex_lock(&writing_mut);
    snapshot_writing_counter++;
    if(snapshot_writing_counter == 1)
        pthread_mutex_lock(&snapshot_mut);
    pthread_mutex_unlock(&writing_mut);


    // concurrency between reading-writing to balance priority to writing
    pthread_mutex_lock(&account_write);
    account_writecount++;

    if (account_writecount == 1)
        pthread_mutex_lock(&account_readtry);
    pthread_mutex_unlock(&account_write);

    pthread_mutex_lock(&account_resource);
}


//********************************************
// function name: unlockSetAccount
// Description: unlock the account for writer
// Parameters: none
// Returns: none
//**************************************************************************************
void Account::unlockSetAccount(){

    pthread_mutex_unlock(&account_resource);
    // concurrency between reading-writing to balance priority to writing
    pthread_mutex_lock(&account_write);
    account_writecount--;
    if(account_writecount == 0)
        pthread_mutex_unlock(&account_readtry);
    pthread_mutex_unlock(&account_write);


    // concurrency between Bank lock (locks all the accounts for writing)
    pthread_mutex_lock(&writing_mut);
    snapshot_writing_counter--;
    if(snapshot_writing_counter == 0)
        pthread_mutex_unlock(&snapshot_mut);
    pthread_mutex_unlock(&writing_mut);
}


//********************************************
// function name: lockGetAccount
// Description: lock the account for readers
// Parameters: none
// Returns: none
//**************************************************************************************
void Account::lockGetAccount(){

    pthread_mutex_lock(&account_readtry);
    pthread_mutex_lock(&account_read);
    account_readcount++;
    if (account_readcount == 1)
        pthread_mutex_lock(&account_resource);
    pthread_mutex_unlock(&account_read);
    pthread_mutex_unlock(&account_readtry);
}


//********************************************
// function name: unlockGetAccount
// Description: unlock the account for readers
// Parameters: none
// Returns: none
//**************************************************************************************
void Account::unlockGetAccount(){
    pthread_mutex_lock(&account_read);
    account_readcount--;
    if (account_readcount == 0)
        pthread_mutex_unlock(&account_resource);
    pthread_mutex_unlock(&account_read);
}

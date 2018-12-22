//
// Created by Adaiah on 14-Dec-18.
//
#include "Account.h"

using namespace std;
//********************************************
// function name: Account
// Description: c'tr, locks are unlocked initially
// Parameters: AtmID - number of current ATM
//             accountID - ID number of the current account
//             password
//             balance - amount to enter to the new account
// Returns: N/A
//**************************************************************************************
Account::Account(int accountId , unsigned short int password , int balance):accountId(accountId),
        password(password), balance(balance), isVIP(false), balance_readcount(0), balance_writecount(0){
    pthread_mutex_init(&balance_read, NULL);
    pthread_mutex_init(&balance_write, NULL);
    pthread_mutex_init(&balance_readtry, NULL);
    pthread_mutex_init(&balance_resource, NULL);
    pthread_mutex_init(&vip_read, NULL);
    pthread_mutex_init(&vip_write, NULL);

}

//********************************************
// function name: ~Account
// Description: d'tr
// Parameters: N/A
// Returns: N/A
//**************************************************************************************
Account::~Account(){
    pthread_mutex_unlock(&balance_read);
    pthread_mutex_unlock(&balance_write);
    pthread_mutex_unlock(&balance_resource);
    pthread_mutex_unlock(&balance_readtry);
    pthread_mutex_unlock(&vip_read);
    pthread_mutex_unlock(&vip_write);


    pthread_mutex_destory(&balance_read);
    pthread_mutex_destory(&balance_write);
    pthread_mutex_destory(&balance_resource);
    pthread_mutex_destory(&balance_readtry);
    pthread_mutex_destory(&vip_read);
    pthread_mutex_destory(&vip_write);
}

//Methods to access data individually

//********************************************
// function name: getBalance
// Description: gets balance in the account
// Parameters: N/A
// Returns: N/A
//**************************************************************************************
unsigned int Account::getBalance(){

    unsigned int curr_balance = 0 ;
    pthread_mutex_lock(&balance_readtry);
    pthread_mutex_lock(&balance_read);
    balance_readcount++;
    if (balance_readcount == 1)
        pthread_mutex_lock(&balance_resource);
    pthread_mutex_unlock(&balance_read);
    pthread_mutex_unlock(&balance_readtry);

    curr_balance = this.balance;

    pthread_mutex_lock(&balance_read);
    balance_readcount--;
    if (balance_readcount == 0)
        pthread_mutex_unlock(&balance_resource);
    pthread_mutex_unlock(&balance_read);

    return curr_balance;
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
// function name: getAccVIP
// Description:
// Parameters:
// Returns:
//**************************************************************************************
bool Account::getAccVIP(){
    bool curr_VIP_status = false ;
    pthread_mutex_lock(&vip_readtry);
    pthread_mutex_lock(&vip_read);
    vip_readcount++;
    if (vip_readcount == 1)
        pthread_mutex_lock(&vip_resource);
    pthread_mutex_unlock(&vip_read);
    pthread_mutex_unlock(&vip_readtry);

    curr_VIP_status = this->isVIP;

    pthread_mutex_lock(&vip_read);
    vip_readcount--;
    if (vip_readcount == 0)
        pthread_mutex_unlock(&vip_resource);
    pthread_mutex_unlock(&vip_read);

    return curr_VIP_status;
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
// function name: printStatus
// Description:
// Parameters:
// Returns:
//**************************************************************************************
void Account::setAccVIP(){

    pthread_mutex_lock(&vip_write);
    vip_writecount++;
    if (vip_writecount == 1)
        pthread_mutex_lock(&vip_readtry);
    pthread_mutex_unlock(&vip_write);

    pthread_mutex_lock(&vip_resource);

    this->isVIP=true;

    pthread_mutex_unlock(&vip_resource);

    pthread_mutex_lock(&vip_write);
    vip_writecount--;
    if(vip_writecount == 0)
        pthread_mutex_unlock(&vip_readtry);
    pthread_mutex_unlock(&vip_write);
}


//********************************************
// function name: addCommission
// Description: add a commission to the commission_taken
// Parameters: int commission
// Returns: void
//**************************************************************************************
void Account::addCommission(int commission){
    this.commision_taken += commission;
}

//********************************************
// function name: setBalance
// Description: set the new balance by adding or decreasing the amount
// Parameters: bool sign , unsigned int amount , int commission_rate
// Returns: int
//**************************************************************************************
int Account::setBalance( bool sign, unsigned int amount, int commission_rate) {
    //sign : true = plus , false = minus, commission_rate=0 unless called upon by bank
    int curr_balance = 0;
    int commission = this->balance*commission_rate;

    // concurrency between the snapshot printing and editing the balances
    pthread_mutex_lock(&writing_mut);
    snapshot_writing_counter++;
    if(snapshot_writing_counter == 1)
        pthread_mutex_lock(&snapshot_mut);
    pthread_mutex_unlock(&writing_mut);


    // concurrency between reading-writing to balance priority to writing
    pthread_mutex_lock(&balance_write);
    balance_writecount++;
    if (balance_writecount == 1)
        pthread_mutex_lock(&balance_readtry);
    pthread_mutex_unlock(&balance_write);

    pthread_mutex_lock(&balance_resource);
    if(!sign){  //decrease
        if ((this.balance - amount - commission) < 0){ //if turn into to negative
            curr_balance = -1;
        } else{
            this.balance = this.balance - amount - commission;
            curr_balance = this.balacne;
        }
    } else{  //increase
        this.balance = this.balance + amount;
        curr_balance = this.balance;
    }

    pthread_mutex_unlock(&balance_resource);

    // concurrency between reading-writing to balance priority to writing
    pthread_mutex_lock(&balance_write);
    balance_writecount--;
    if(balance_writecount == 0)
        pthread_mutex_unlock(&balance_readtry);
    pthread_mutex_unlock(&balance_write);

    if(commission_rate>0 && curr_balance != -1) { //log commission charging
        pthread_mutex_lock(log_write_mut);
        logfile<<"Bank: commision of " << commission_rate << " % " << "were charged, the bank gained " << commission << " $ from account " << getAccountId() << endl;
        pthread_mutex_unlock(log_write_mut);
        this.addCommission(commission);

    }

    // concurrency between the snapshot printing and editing the balances
    pthread_mutex_lock(&writing_mut);
    snapshot_writing_counter--;
    if(snapshot_writing_counter == 0)
        pthread_mutex_unlock(&snapshot_mut);
    pthread_mutex_unlock(&writing_mut);

    return curr_balance;

}
//********************************************
// function name: printAccount
// Description:
// Parameters:
// Returns:
//**************************************************************************************
unsigned int Account::printAccount(){
    unsigned int curr_balance = this.getBalance();
    cout <<"Account "<< this.getAccountId() <<": Balance - " << curr_balance
        << " $ , Account Password - " << this.getPassowrd() << endl;
    return curr_balance;
}



//********************************************
// function name: getCommissionTaken
// Description: get the commission which been taken from the account and zeroing it
// Parameters: none
// Returns: int
//**************************************************************************************
int Account::getCommissionTaken(){
    int commission = 0;
    commission = this.commision_taken;
    this.commision_taken = 0;
    return commission;
}
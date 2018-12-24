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
        password(password), balance(balance), isVIP(false), balance_readcount(0), balance_writecount(0),
        commission_taken(0), vip_readcount(0), vip_writecount(0){
    pthread_mutex_init(&balance_read, NULL);
    pthread_mutex_init(&balance_write, NULL);
    pthread_mutex_init(&balance_readtry, NULL);
    pthread_mutex_init(&balance_resource, NULL);
    pthread_mutex_init(&vip_read, NULL);
    pthread_mutex_init(&vip_readtry, NULL);
    pthread_mutex_init(&vip_resource, NULL);
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
    pthread_mutex_unlock(&vip_readtry);
    pthread_mutex_unlock(&vip_resource);


    pthread_mutex_destroy(&balance_read);
    pthread_mutex_destroy(&balance_write);
    pthread_mutex_destroy(&balance_resource);
    pthread_mutex_destroy(&balance_readtry);
    pthread_mutex_destroy(&vip_read);
    pthread_mutex_destroy(&vip_write);
    pthread_mutex_destroy(&vip_readtry);
    pthread_mutex_destroy(&vip_resource);
}

//Methods to access data individually

//********************************************
// function name: getBalance
// Description: gets balance in the account
// Parameters: N/A
// Returns: N/A
//**************************************************************************************
unsigned int Account::getBalance(bool bank_or_atm, unsigned int atm_id){

    unsigned int curr_balance = 0 ;
    pthread_mutex_lock(&balance_readtry);
    pthread_mutex_lock(&balance_read);
    balance_readcount++;
    if (balance_readcount == 1)
        pthread_mutex_lock(&balance_resource);
    pthread_mutex_unlock(&balance_read);
    pthread_mutex_unlock(&balance_readtry);

    curr_balance = this->balance;

    if(bank_or_atm) {
        sleep(1);
        pthread_mutex_lock(&log_write_mut);
        logfile << atm_id << ": Account " << this->accountId << " balance is " << curr_balance << endl;
        pthread_mutex_unlock(&log_write_mut);
    }

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
// Description: retuns whether the aacount is VIP or not
// Parameters: NONE
// Returns: true- VIP, false- not VIP
//**************************************************************************************
bool Account::getAccVIP(){

    bool curr_VIP_status = false ;
    pthread_mutex_lock(&this->vip_readtry);
    pthread_mutex_lock(&this->vip_read);
    vip_readcount++;
    if (vip_readcount == 1)
        pthread_mutex_lock(&this->vip_resource);
    pthread_mutex_unlock(&this->vip_read);
    pthread_mutex_unlock(&this->vip_readtry);


    curr_VIP_status = this->isVIP;
    pthread_mutex_lock(&this->vip_read);
    vip_readcount--;
    if (vip_readcount == 0)
        pthread_mutex_unlock(&this->vip_resource);
    pthread_mutex_unlock(&this->vip_read);

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
// function name: setAccVIP
// Description: turns the account to VIP
// Parameters: NONE
// Returns: NULL
//**************************************************************************************
void Account::setAccVIP(){

    pthread_mutex_lock(&vip_write);
    vip_writecount++;
    if (vip_writecount == 1)
        pthread_mutex_lock(&vip_readtry);
    pthread_mutex_unlock(&vip_write);

    pthread_mutex_lock(&vip_resource);

    this->isVIP=true;
    sleep(1);

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
    this->commission_taken += commission;
}

//********************************************
// function name: setBalance
// Description: set the new balance by adding or decreasing the amount
// Parameters: bool sign , unsigned int amount , int commission_rate
// Returns: int
//**************************************************************************************
int Account::setBalance(ATM_Action atm_action, int amount, double commission_rate, unsigned int atm_id ) {
    int curr_balance = 0;
    this->lockSetBalance();
    double commission_d =balance*(commission_rate/100);
    int commission = commission_d;
    if ( commission + 0.5 < commission_d )
        commission =commission+1;
    if((atm_action == WITHDRAW) || (atm_action == COMMISSION) ){  //decrease
        if ((balance - amount - commission) < 0){ //if turn into to negative
            curr_balance = -1;
        } else{
            balance = balance - amount - commission;
            curr_balance = balance;
        }
    } else{  //DEPOSIT = increase
        balance = balance + amount;
        curr_balance = balance;
    }
    if((atm_action == WITHDRAW) || (atm_action == DEPOSIT)) sleep(1);

    if((atm_action == COMMISSION) && (curr_balance >0)) {  //log commission charging
        pthread_mutex_lock(&log_write_mut);
        logfile<<"Bank: commission of " << commission_rate << " % " << "were charged, the bank gained " << commission << " $ from account " << getAccountId() << endl;
        pthread_mutex_unlock(&log_write_mut);
        this->addCommission(commission);
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


    this->unlockSetBalance();

    return curr_balance;
}
//********************************************
// function name: printAccount
// Description:
// Parameters:
// Returns:
//**************************************************************************************
unsigned int Account::printAccount(){
    unsigned int curr_balance = this->getBalance(false, 0);
    cout <<"Account "<< this->getAccountId() <<": Balance - " << curr_balance
        << " $ , Account Password - " << this->password << endl;
    return curr_balance;
}



//********************************************
// function name: getCommissionTaken
// Description: get the commission which been taken from the account and zeroing it
// Parameters: none
// Returns: int
//**************************************************************************************
int Account::getCommissionTaken(){
    int commission = this->commission_taken;
    this->commission_taken = 0;
    return commission;
}


void Account::lockSetBalance(){
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
}

void Account::unlockSetBalance(){

    pthread_mutex_unlock(&balance_resource);
    // concurrency between reading-writing to balance priority to writing
    pthread_mutex_lock(&balance_write);
    balance_writecount--;
    if(balance_writecount == 0)
        pthread_mutex_unlock(&balance_readtry);
    pthread_mutex_unlock(&balance_write);


    // concurrency between the snapshot printing and editing the balances
    pthread_mutex_lock(&writing_mut);
    snapshot_writing_counter--;
    if(snapshot_writing_counter == 0)
        pthread_mutex_unlock(&snapshot_mut);
    pthread_mutex_unlock(&writing_mut);
}



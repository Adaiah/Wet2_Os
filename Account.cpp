//
// Created by Adaiah on 14-Dec-18.
//
#include "Account.h"

Account::Account(int accountId , int password , int balance):accountId(accountId),
        password(password), balance(balance), isVIP(false), balance_readcount(0), balance_writecount(0){
    pthread_mutex_init(&balance_read, NULL);
    pthread_mutex_init(&balance_write, NULL);
    pthread_mutex_init(&balance_readtry, NULL);
    pthread_mutex_init(&balance_resource, NULL);
    pthread_mutex_init(&vip_read, NULL);
    pthread_mutex_init(&vip_write, NULL);

}

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

int Account:: getAccountId()

bool Account::getAccVIP();

unsigned short int getPassword();

int Account::checkPassword(unsigned short int password);

//Methods to change  the data
void Account::setAccVIP();

int Account::setBalance( bool sign, unsigned int amount) { //sign : true = plus , false = minus
    int curr_balance = 0;

    pthread_mutex_lock(&balance_write);
    balance_writecount++;
    if (balance_writecount == 1)
        pthread_mutex_lock(&balance_readtry);
    pthread_mutex_unlock(&balance_write);

    pthread_mutex_lock(&balance_resource);
    if(!sign){  //decrease
        if ((this.balance - amount) < 0){ //if turn into to negative
            curr_balance = -1;
        } else{
            this.balance = this.balance - amount;
            curr_balance = this.balacne;
        }
    } else{  //increase
        this.balance = this.balance + amount;
        curr_balance = this.balance;
    }

    pthread_mutex_unlock(&balance_resource);

    pthread_mutex_lock(&balance_write);
    balance_writecount--;
    if(balance_writecount == 0)
        pthread_mutex_unlock(&balance_readtry);
    pthread_mutex_unlock(&balance_write);

}
unsigned int printAccount(){
    unsigned int curr_balance = this.getBalance();
    cout <<"Account "<< this.getAccountId() <<": Balance - " << curr_balance
        << " $ , Account Password - " << this.getPassowrd() << endl;
    return curr_balance;
}

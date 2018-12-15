//
// Created by Adaiah on 14-Dec-18.
//
#include "Account.h"

Account::Account(int accountId , int password , int balance):accountId(accountId), password(password), balance(balance), isVIP(false){
    pthread_mutex_init(&balance_read, NULL);
    pthread_mutex_init(&balance_write, NULL);
    pthread_mutex_init(&vip_read, NULL);
    pthread_mutex_init(&vip_write, NULL);

}

Account::~Account(){
    pthread_mutex_unlock(&balance_read);
    pthread_mutex_unlock(&balance_write);
    pthread_mutex_unlock(&vip_read);
    pthread_mutex_unlock(&vip_write);

    pthread_mutex_destory(&balance_read);
    pthread_mutex_destory(&balance_write);
    pthread_mutex_destory(&vip_read);
    pthread_mutex_destory(&vip_write);
}

//Methods to access data individually
int Account::getBalance(){

}

int Account:: getAccountId()

bool Account::getAccVIP();

int Account::checkPassword(int password);

//Methods to change  the data
void Account::setAccVIP();

int Account::setBalance( bool sign, unsigned int amount); //sign : true = plus , false = minus
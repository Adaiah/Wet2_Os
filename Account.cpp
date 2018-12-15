//
// Created by Adaiah on 14-Dec-18.
//
#include "Account.h"

Account::Account(int accountId , int password , int amount):accountId(accountId), password(password), amount(amount), isVIP(false){
    pthread_mutex_init(&amount_read, NULL);
    pthread_mutex_init(&amount_write, NULL);
    pthread_mutex_init(&vip_read, NULL);
    pthread_mutex_init(&vip_write, NULL);

}

Account::~Account(){
    pthread_mutex_unlock(&amount_read);
    pthread_mutex_unlock(&amount_write);
    pthread_mutex_unlock(&vip_read);
    pthread_mutex_unlock(&vip_write);

    pthread_mutex_destory(&amount_read);
    pthread_mutex_destory(&amount_write);
    pthread_mutex_destory(&vip_read);
    pthread_mutex_destory(&vip_write);
}

//Methods to access data individually
int Account::getAmount(){

}

int Account:: getAccountId()

bool Account::getAccVIP();

int Account::getPassword();

//Methods to change  the data
bool Account::setAccVIP();

bool Account::setAmount( bool sign, int entered_amount);
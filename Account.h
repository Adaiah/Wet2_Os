//
// Created by Adaiah on 14-Dec-18.
//

#ifndef WET2_ACCOUNT_H
#define WET2_ACCOUNT_H

#include <pthread.h>
#include <type_traits>
#include <iostream>
#include <fstream>

extern unsigned int bank_sum;
extern pthread_mutex_t log_write_mut;
extern ofstream logfile;


class Account {
private:
    int accountId;
    unsigned short int password;
    unsigned int balance;
    pthread_mutex_t balance_read;
    pthread_mutex_t balance_write;
    pthread_mutex_t balance_readtry;
    pthread_mutex_t balance_resource;
    int balance_readcount, balance_writecount;
    bool  isVIP;
    pthread_mutex_t vip_read;
    pthread_mutex_t vip_write;

public:
    //constructor
    Account(int accountId , unsigned short int password , int balance);

    //destructor
    ~Account();

    //Methods to access data individually
    int getAccountId();

    unsigned int getBalance();

    bool getAccVIP();

    unsigned short int getPassword();

    bool checkPassword(unsigned short int password);

    //Methods to change  the data
    void setAccVIP();
//todo: added commission rate to function args
    int setBalance( bool sign, unsigned int amount, int commission_rate); //sign true = plus, false = minus

    unsigned int printAccount();
};
#endif //WET2_ACCOUNT_H

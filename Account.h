//
// Created by Adaiah on 14-Dec-18.
//

#ifndef WET2_ACCOUNT_H
#define WET2_ACCOUNT_H

#include <pthread.h>
#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;

extern pthread_mutex_t log_write_mut;
extern pthread_mutex_t snapshot_mut;
extern pthread_mutex_t writing_mut;
extern int snapshot_writing_counter;
extern ofstream logfile;

typedef enum{
    WITHDRAW,
    DEPOSIT,
    COMMISSION
}ATM_Action;



class Account {
private:
    int accountId;
    unsigned short int password;
    unsigned int commission_taken;
    bool isVIP;
    pthread_mutex_t vip_read;
    pthread_mutex_t vip_write;
    pthread_mutex_t vip_readtry;
    pthread_mutex_t vip_resource;
    int vip_readcount, vip_writecount;
    int balance_readcount, balance_writecount;
    pthread_mutex_t balance_read;
    pthread_mutex_t balance_write;
    pthread_mutex_t balance_readtry;
    pthread_mutex_t balance_resource;



public:
    unsigned int balance;

    //constructor
    Account(int accountId=0, unsigned short int password=0, int balance=0);

    //destructor
    ~Account();

    //Methods to access data individually
    int getAccountId();

    unsigned int getBalance( bool bank_or_Atm, unsigned int atm_id); //bank = false, atm = true

    bool getAccVIP();

    unsigned short int getPassword();

    bool checkPassword(unsigned short int password);

    int getCommissionTaken();

        //Methods to change  the data
    void setAccVIP();
    void addCommission(int commission);

    int setBalance(ATM_Action atm_action, unsigned int amount, int commission_rate ,unsigned int atm_id );

    unsigned int printAccount();

    void lockSetBalance();

    void unlockSetBalance();
};

#endif //WET2_ACCOUNT_H

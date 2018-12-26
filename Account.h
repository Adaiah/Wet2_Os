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
}ATM_Action;



class Account {
private:
    int accountId;
    unsigned short int password;
    unsigned int commission_taken;
    bool isVIP;
    int account_readcount, account_writecount;
    pthread_mutex_t account_read;
    pthread_mutex_t account_write;
    pthread_mutex_t account_readtry;
    pthread_mutex_t account_resource;



public:
    int balance;

    //constructor
    Account(int accountId=0, unsigned short int password=0, int balance=0);

    //destructor
    ~Account();

    //Methods to access data individually
    int getAccountId();

    unsigned short int getPassword();

    bool checkPassword(unsigned short int password);

    int getCommissionTaken();

    //Methods to change  the data

    void setAccVIP();

    void addCommission(int commission);

    void takeCommission( double commission_rate);

    int setBalance(ATM_Action atm_action, int amount,unsigned int atm_id );

    //print function

    void printAccount();

    void printBalance(unsigned int atm_id);

    // lock-unlock function

    void lockSetAccount();

    void unlockSetAccount();

    void lockGetAccount();

    void unlockGetAccount();
};

#endif //WET2_ACCOUNT_H

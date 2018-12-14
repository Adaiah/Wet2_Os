//
// Created by Adaiah on 14-Dec-18.
//

#ifndef WET2_ACCOUNT_H
#define WET2_ACCOUNT_H

#include <pthread.h>

class Account {
private:
    int accountId;
    int password;
    int amount;
    pthread_mutex_t amount_read;
    pthread_mutex_t amount_write;
    bool  isVIP;
    pthread_mutex_t vip_read;
    pthread_mutex_t vip_write;

public:
    //constructor
    Account(int accountId , int password , int amount);

    //destructor
    ~Account();

    //Methods to access data individually
    int getAccountId();

    int getAmount();

    bool getAccVIP();

    int getPassword();

    //Methods to change  the data
    void setAccVIP();

    void setAmount();
};
#endif //WET2_ACCOUNT_H

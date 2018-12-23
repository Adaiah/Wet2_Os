//
// Created by Adaiah on 14-Dec-18.
//

#ifndef WET2_BANK_H
#define WET2_BANK_H

#include "Account.h"
#include <stdio.h>
#include <unistd.h>
#include <map>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <stdlib.h>

using namespace std;

extern pthread_mutex_t snapshot_mut;
extern pthread_mutex_t writing_mut;
extern int snapshot_writing_counter;
extern bool finished_all_actions;

extern map<int, Account> bank_accounts;
extern ofstream logfile;

typedef struct{
    Account* curr_Account;
    int commission_rate;
}Comm_args;
void *miniMainBank(void* args);

#endif //WET2_BANK_H

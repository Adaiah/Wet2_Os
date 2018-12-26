//
// Created by Adaiah on 14-Dec-18.
//

#ifndef WET2_ATM_H
#define WET2_ATM_H

#include "Account.h"
#include <pthread.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <unistd.h>

#define PLUS true
#define MINUS false


using namespace std;

extern ofstream logfile;
extern map<int, Account> bank_accounts;
extern pthread_mutex_t log_write_mut;
extern pthread_mutex_t bank_account_mut;
extern bool finished_all_actions;

typedef struct{
    int tot_num_of_atm;
    vector<string> input_files;
}Atm_args;

typedef struct{
    int ATM_ID;
    string input_file; //
}atm_input_output;


void* miniMainATM(void*);

#endif //WET2_ATM_H

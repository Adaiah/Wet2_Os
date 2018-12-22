//
// Created by Adaiah on 14-Dec-18.
//

#ifndef WET2_ATM_H
#define WET2_ATM_H

#include <streamio>// todo: What is dis?
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


using namespace std;

#define PLUS true
#define MINUS false


extern ofstream logfile;

typedef struct{
    int tot_num_of_atm;
    vector<string> input_files;
}Atm_args;

typedef struct{
    int ATM_ID;
    string input_file; //
}atm_input_output;


extern map<int, Account> bank_accounts;
extern pthread_mutex_t log_write_mut;

void* miniMainATM(void*);

#endif //WET2_ATM_H

//
// Created by Adaiah on 14-Dec-18.
//

#ifndef WET2_ATM_H
#define WET2_ATM_H

#include <streamio>
#include "Account.h"
#include <pthread.h>
#include <string>
#include <vector>

using namespace std

#define PLUS true
#define MINUS false

#typedef struct{
    int num_of_atm;
    string log_file_name;
    vector<string> input_files;
}Atm_args;

#typedef struct{
    string log_file_name;
    string input_file;
}atm_input_output;


extern map<int, Account> bank_accounts;
void* miniMainATM(void*);

#endif //WET2_ATM_H

//
// Created by Adaiah on 14-Dec-18.
//

#ifndef WET2_BANK_H
#define WET2_BANK_H

#include "Account.h"
#include <stdio.h>
#include <streamio>
#include <unistd.h>
#include <map>
#include <iostream>
#include <fstream>
#include <pthread.h>


using namespace std;

extern map<int, Account> bank_accounts;
extern ofstream logfile;
//

void *miniMainBank(void* args);

#endif //WET2_BANK_H

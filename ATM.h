//
// Created by Adaiah on 14-Dec-18.
//

#ifndef WET2_ATM_H
#define WET2_ATM_H

#include <streamio>
#include "Account.h"
#include <pthread.h>

#define PLUS true
#define MINUS false

using namespace std

extern map<int, Account> bank_accounts;
void* miniMainATM(void*);

#endif //WET2_ATM_H

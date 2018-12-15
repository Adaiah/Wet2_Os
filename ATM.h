//
// Created by Adaiah on 14-Dec-18.
//

#ifndef WET2_ATM_H
#define WET2_ATM_H

#include "Account.h"
#include <map>

extern map<int, Account> bank_accounts;
void* miniMainATM(void* args);

#endif //WET2_ATM_H

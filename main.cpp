#include <iostream>
#include "ATM.h"
#include "Bank.h"
#include "Account.h"
#include <pthread.h>
#include <map>

using namespace std;

//global map
map<int, Account> bank_accounts;

int main(int argc, char* argv[]) {

    pthread_t bank_thread;
    pthread_t atm_thread;

    //todo: insert here the building of the passing arguments for the atm function
    //atm_args = ...

    thread_create(&bank_thread, NULL, miniMainBank, NULL);
    thread_create(&atm_thread, NULL, miniMainATM, (void *)atm_args);

    pthread_join(bank_thread, NULL);
    pthread_join(atm_thread, NULL);

    return 0;
}


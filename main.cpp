#include <iostream>
#include "ATM.h"
#include "Bank.h"
#include "Account.h"
#include <pthread.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>

#define NUM_OF_ATM 1

using namespace std;

//global map
map<int, Account> bank_accounts;

int main(int argc, char* argv[]) {

    pthread_t bank_thread;
    pthread_t atm_thread;

    vector<string>input_files( argv+2, argv+argc);
    Atm_args* atm_args = new Atm_args ;
    atm_args -> log_file_name = "log.txt";
    atm_args -> num_of_atm = atoi(argv[NUM_OF_ATM]);
    atm_args -> input_files = input_files;

    thread_create(&bank_thread, NULL, miniMainBank, NULL);
    thread_create(&atm_thread, NULL, miniMainATM, (void *)atm_args);

    pthread_join(bank_thread, NULL);
    pthread_join(atm_thread, NULL);

    delete atm_args;
    return 0;
}


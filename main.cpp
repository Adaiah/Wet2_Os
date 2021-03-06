#include <iostream>
#include <fstream>
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

//global variables
map<int, Account> bank_accounts;
pthread_mutex_t log_write_mut;
pthread_mutex_t snapshot_mut;
pthread_mutex_t bank_account_mut;
bool finished_all_actions;
ofstream logfile;

int main(int argc, char* argv[]) {
    if(argc<2) { // too few arguments
        logfile<< "illegal arguments" << endl;
        return 0;
    }

    finished_all_actions = false;
    pthread_t bank_thread;
    pthread_t atm_thread;

    logfile.open("log.txt"); // creating log file

    vector<string>input_files( argv+2, argv+argc);
    Atm_args* atm_args = new Atm_args ;
    atm_args -> tot_num_of_atm = atoi(argv[NUM_OF_ATM]);
    atm_args -> input_files = input_files;
    if(input_files.size()!= atm_args->tot_num_of_atm ){ // if number of files does not match number of ATMs
        logfile<< "illegal arguments" << endl;
        return 0;
    }
    pthread_mutex_init(&log_write_mut, NULL);
    pthread_mutex_init(&snapshot_mut, NULL);
    pthread_mutex_init(&bank_account_mut, NULL);

    map<int, Account>::iterator it;
    pthread_create(&bank_thread, NULL, miniMainBank, NULL);
    pthread_create(&atm_thread, NULL, miniMainATM, (void *)atm_args);

    pthread_join(bank_thread, NULL);
    pthread_join(atm_thread, NULL);
    logfile.close();

    if(!pthread_mutex_trylock(&log_write_mut))
        pthread_mutex_unlock(&log_write_mut);

    if(!pthread_mutex_trylock(&snapshot_mut))
        pthread_mutex_unlock(&snapshot_mut);

    if(!pthread_mutex_trylock(&bank_account_mut))
        pthread_mutex_unlock(&bank_account_mut);

    pthread_mutex_destroy(&log_write_mut);
    pthread_mutex_destroy(&snapshot_mut);
    pthread_mutex_destroy(&bank_account_mut);
    delete atm_args;
    return 0;
}

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
pthread_mutex_t writing_mut;
int snapshot_writing_counter;
bool finished_all_actions;
ofstream logfile;

int main(int argc, char* argv[]) {
    finished_all_actions = false;
    pthread_t bank_thread;
    pthread_t atm_thread;
    logfile.open("log.txt");
    vector<string>input_files( argv+2, argv+argc);
    Atm_args* atm_args = new Atm_args ;
    atm_args -> tot_num_of_atm = atoi(argv[NUM_OF_ATM]);
    atm_args -> input_files = input_files;
    pthread_mutex_init(&log_write_mut, NULL);
    pthread_mutex_init(&snapshot_mut, NULL);
    pthread_mutex_init(&writing_mut, NULL);

    map<int, Account>::iterator it;
    pthread_create(&bank_thread, NULL, miniMainBank, NULL);
    pthread_create(&atm_thread, NULL, miniMainATM, (void *)atm_args);

    pthread_join(bank_thread, NULL);
    pthread_join(atm_thread, NULL);
    logfile.close(); //todo: ??

    pthread_mutex_unlock(&log_write_mut);
    pthread_mutex_unlock(&snapshot_mut);
    pthread_mutex_unlock(&writing_mut);

    pthread_mutex_destroy(&log_write_mut);
    pthread_mutex_destroy(&snapshot_mut);
    pthread_mutex_destroy(&writing_mut);
    delete atm_args;
    return 0;
}

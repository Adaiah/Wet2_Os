//
// Created by Adaiah on 15-Dec-18.
//
#include "Bank.h"

//********************************************
// function name: printStatus
// Description:
// Parameters:
// Returns:
//**************************************************************************************
void *printStatus(void* args){
    map<int, Account>::iterator it;
    unsigned int bank_sum =0;
    while(1){
        pthread_mutex_lock(&snapshot_mut);
        printf("\033[2j");
        printf("\033[1;1H");
        cout << "Current Bank Status" << endl;
        //todo: check if the map is ascending or descending
        for( it = bank_accounts.begin(); it != bank_accounts.end(); it++){
            it->printAccount();
            bank_sum += it->getCommissionTaken();
        }
        cout << "The Bank has "<< bank_sum << " $"<<endl;
        pthread_mutex_unlock(&snapshot_mut);
        usleep(500000); //sleep for half a second
    }
}
//********************************************
// function name: commissions
// Description: the function that runs on the commission thread. takes commission every 3 sec
// Parameters: None
// Returns: NULL
//**************************************************************************************
void *commissions (void* args){
    map<int, Account>::iterator it;
    int commission_rate;
    while(1) {
        commission_rate = rand() % 3 + 2; //TODO: MAKE SURE THIS IS THE RIGHT DEFINITION
        for (it = bank_accounts.begin(); it < bank_accounts.end(); it++) {
            if ((it->getAccVIP()) == false) {   //account not VIP
                setBalance(false, 0, commission_rate);
            }
        }
        sleep(3);
    }

}
//********************************************
// function name: miniMainBank
// Description: creates the threads for the bank. a thread for printing and another for commission
// Parameters: None
// Returns: NULL
//**************************************************************************************
void *miniMainBank(void* args) {

    pthread_t printing_thread;
    pthread_t commission_thread;

    thread_create(&commission_thread, NULL, commisions, NULL);
    thread_create(&printing_thread, NULL, printStatus, NULL);

    pthread_join(printing_thread, NULL);
    pthread_join(commission_thread, NULL);
    return NULL; //todo : check what to return
}


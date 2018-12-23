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
    while(!finished_all_actions){
        pthread_mutex_lock(&snapshot_mut);
        printf("\033[2J");
        printf("\033[1;1H");
        cout << "Current Bank Status" << endl;
        //todo: check if the map is ascending or descending
        for( it = bank_accounts.begin(); it != bank_accounts.end(); it++){
            it->second.printAccount();
            bank_sum += it->second.getCommissionTaken();
        }
        cout << "The Bank has "<< bank_sum << " $"<<endl;
        pthread_mutex_unlock(&snapshot_mut);
        usleep(500000); //sleep for half a second
    }
    //final snapshot. there are no more ATM but commission can be still on
    pthread_mutex_lock(&snapshot_mut);
    printf("\033[2J");
    printf("\033[1;1H");
    cout << "Current Bank Status" << endl;
    //todo: check if the map is ascending or descending
    for( it = bank_accounts.begin(); it != bank_accounts.end(); it++){
        it->second.printAccount();
        bank_sum += it->second.getCommissionTaken();
    }
    cout << "The Bank has "<< bank_sum << " $"<<endl;
    pthread_mutex_unlock(&snapshot_mut);
    pthread_exit(NULL);
}

void *commissioner(void *args) {
    Comm_args *comm_args= (Comm_args*) args;
    int curr_commission= comm_args->commission_rate;
    comm_args->curr_Account->setBalance(false,0,curr_commission,false);
}
//********************************************
// function name: commissions
// Description: the function that runs on the commission thread. takes commission every 3 sec
// Parameters: None
// Returns: NULL
//**************************************************************************************
void *getCommissions (void* args){
    map<int, Account>::iterator it;
    int commission_rate;
    int temp,rc,i ;
    int numberOfAccounts=0;
    while(!finished_all_actions) {
        numberOfAccounts=bank_accounts.size();
        pthread_t AccountCommission[numberOfAccounts];
        commission_rate = rand() % 3 + 2; //TODO: MAKE SURE THIS IS THE RIGHT DEFINITION
        if(numberOfAccounts>0){
            for(i=0,it = bank_accounts.begin(); it != bank_accounts.end(), i<numberOfAccounts; it++,i++) {
                if ((it->second.getAccVIP()) == false) {   //account not VIP
                    Comm_args* comm_args= new Comm_args;
                    comm_args->commission_rate=commission_rate;
                    comm_args->curr_Account=&it->second;
                    if (rc = pthread_create(&AccountCommission[i], NULL, commissioner, (void *)comm_args)) {
                        cout << "error: pthread_create, rc: " << rc << endl;
                        pthread_exit(NULL);
                    }
                }
            }
            for (i=0 ; i < numberOfAccounts; ++i){
                   pthread_join(AccountCommission[i], NULL);
               }
            }
        sleep(3);
    }
    pthread_exit(NULL);
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

    pthread_create(&printing_thread, NULL, printStatus, NULL);
    pthread_create(&commission_thread, NULL, getCommissions, NULL);

    pthread_join(printing_thread, NULL);
    pthread_join(commission_thread, NULL);

    pthread_exit(NULL);
}


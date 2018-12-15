//
// Created by Adaiah on 15-Dec-18.
//
#include "Bank.h"

void *printStatus(void* args){
    map<int, Account>::iterator it;
    while(1){
        unsigned int bank_sum = 0;
        printf("\033[2j");
        printf("\033[1;1H");
        cout << "Current Bank Status" << endl;
        //todo: check if the map is ascending or descending
        for( it = bank_accounts.begin(); it != bank_accounts.end(); it++){
            bank_sum += it->printAccount();
        }
        cout << "The Bank has "<< bank_sum << " $"<<endl;
        usleep(500000); //sleep for half a second
    }
}
void *commisions ();

void *miniMainBank(void* args) {

}

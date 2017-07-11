//
//  register.hpp
//  microsim
//
//  Created by David Brown on 09/07/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#ifndef register_hpp
#define register_hpp

#include <stdio.h>
#include <vector>

class Firm;
class Worker;
class Account;

class Register
{
    
private:
    
    static Register *_instance;
    
    Register();

    std::vector<Firm*> firms;
    std::vector<Worker*> workers;
    
    int num_hired;
    int num_fired;
    int num_just_fired;

public:
    
    static Register *Instance();
    ~Register();
    
    enum Status {
        any,
        employed,
        unemployed
    };
    
    enum Reason {
        wages,
        benefits,
        bonus
    };
    
    void init();                // Initialises everything for a new period. The
                                // Government may then make some changes prior
                                // to triggering the system (e.g. making grants,
                                // which will change some balances)
    void trigger(int period);
    
    // Firms
    
    Firm *createFirm();
    Firm *selectRandomFirm();
    
    size_t getNumFirms();

    // Totals
    
    int getProdBal();
    
    int getGrantsRecd();
    int getDednsPaid();
    int getWagesPaid();
    int getBonusesPaid();
    int getSalesReceipts();
    int getSalesTaxPaid();
    
    // Workers
    
    Worker *hire(Firm *employer, int period);
    int hireSome(Firm *employer, int period, int n);
    
    void fire(Worker*, int period);
    
    size_t getNumWorkers();
    
    int getNumEmployed();
    int getNumUnemployed();
    int getNumEmployedBy(Firm*);
    
    int payWorkers(int amount,
                   int max_tot,
                   Account *source,
                   Reason reason = wages,
                   int period = 0  // immaterial unless reason == wages
                   );
    int getNumJustFired();          // This is reset each time payWorkers is called
                                // with reason 'wages'.

    // Totals
    
    int getNumHired();
    int getNumFired();
    
    int getWorkersBal(Status status = any);
    
    int getIncTaxPaid();
    int getPurchasesMade();
    
    int getWagesRecd();
    int getBenefitsRecd();
    
};

#endif /* register_hpp */

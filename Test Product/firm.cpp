//
//  firm.cpp
//  Test Product
//
//  Created by David Brown on 06/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "account.hpp"
#include <cassert>

Firm::Firm(int standard_wage)
{
    std_wage = standard_wage;
}

void Firm::trigger(int period)
{
    std::cout << "Firm triggered with balance " << balance << "\n";
    
    // Firm must pay all its employees, hiring and firing as
    // necessary, and then trigger each employee in the resulting
    // list.
    
    // Pay wexisting employees and calculate the total committed wage bill
    int committed = 0;
    for (auto it : employees) {
        int wage = it->getWage();
        std::cout << "Firm transferring " << wage << " to employee\n";
        if (!transferTo(it, wage)) {
            std::cout << "Firm: insufficient funds to pay employee\n";
        }
        std::cout << "Firm new balance is " << balance << "\n";
        committed += wage;
    }

    // Trigger all the employees (note that we pay them all before
    // we trigger any of them.
    for (auto it : employees) {
        it->trigger(period);
    }

    if (balance - committed >= std_wage) {
        assert(std_wage>0);
        int num_hires = ((balance - committed) / std_wage);
        std::cout << "Firm: hiring " << num_hires << " new employees" << "\n";
        Pool *pool = Pool::Instance();
        for (int i = 0; i < num_hires; i++) {
            employees.push_back(pool->hire(std_wage, this));
        }
    }
}

void Firm::credit(int amount)
{
    Account::credit(amount);
    std::cout << "Firm credited, balance: " << balance << "\n";
}


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
    for (auto it : employees)
    {
        if (it->isEmployed())   // check that we haven't fired this employee
        {
            int wage = it->getWage();
            std::cout << "Firm transferring " << wage << " to employee\n";
            if (transferTo(it, wage))
            {
                std::cout << "Firm new balance is " << balance << "\n";
                committed += wage;
            }
            else
            {
                std::cout << "Firm: insufficient funds to pay employee -- firing\n";
                pool->fire(it);
            }
        }
    }

    // Trigger all the employees (note that we pay them all before
    // we trigger any of them. We only trigger them if we still employ them,
    // otherwise it's the Pool's responsibility.
    // TO DO: add trigger to Pool.
    for (auto it : employees) {
        if (it->isEmployed()) {
            it->trigger(period);
        }
    }

    // If we have funds left over, hire some more employees
    if (balance - committed >= std_wage) {
        assert(std_wage>0);
        int num_hires = ((balance - committed) / std_wage);
        std::cout << "Firm: hiring " << num_hires << " new employees" << "\n";
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

Firm::~Firm()
{
    // We should free the space taken up by any Workers in the employees
    // vector. Where they have been fired we do not free the space as this
    // will be done by Pool when tidying up its 'available' vector (check this!)
    // ...
}


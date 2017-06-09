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
    std::cout   << "Firm: triggered with balance "
                << balance
                << "\n";
    
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
            int dedns = (wage * settings->dedns) / 100;
            std::cout   << "Firm: transferring wage "
                        << (wage - dedns)
                        << " to employee and dedns "
                        << dedns
                        << " to government\n";
            if (wage <= balance)
            {
                transferTo(it, wage - dedns);
                transferTo(Government::Instance(), dedns);
                committed += wage;
                std::cout << "Firm: new balance is " << balance << "\n";
            }
            else
            {
                std::cout << "Firm: insufficient funds to pay employee -- firing\n";
                pool->fire(it);
            }
        }
    }

    // Trigger all the employees (note that we pay them all before
    // we trigger any of them, and we only trigger them if we still
    // employ them -- otherwise it's the Pool's responsibility.
    for (auto it : employees)
    {
        if (it->isEmployed()) {
            it->trigger(period);
        }
    }

    // If we have funds left over, hire some more employees
    
    assert(std_wage>0);

    int num_hires = (((balance * settings->prop_con) / 100) - committed) / std_wage;
    if (num_hires > 0)
    {
        std::cout   << "Firm: hiring "
                    << num_hires
                    << " new employees\n";
        
        for (int i = 0; i < num_hires; i++) {
            employees.push_back(pool->hire(std_wage, this));
        }
    }
}

void Firm::credit(int amount)
{
    Account::credit(amount);
    std::cout   << "Firm credited, balance: "
                << balance
                << "\n";
}

Firm::~Firm()
{
    // We should free the space taken up by any Workers in the employees
    // vector. Where they have been fired we do not free the space as this
    // will be done by Pool when tidying up its 'available' vector (check this!)
    for (auto it : employees) {
        delete it;
        employees.pop_back();
    }
}


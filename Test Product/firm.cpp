//
//  firm.cpp
//  Test Product
//
//  Created by David Brown on 06/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "account.hpp"

Firm::Firm(int standard_wage)
{
    std_wage = standard_wage;
}

void Firm::trigger(int period)
{
    std::cout << "Firm triggered\n";
    
    // Firm must pay all its employees, hiring and firing as
    // necessary, and then trigger each employee in the resulting
    // list.
    
    // Pay wexisting employees and calculate the total
    int total = 0;
    for (auto it : employees) {
        int wage = it->getWage();
        transferTo(it, wage);
        total += wage;
        it->trigger(period);
    }
    
    if (balance > std_wage) {
        // We can afford to take on more employees
        // ...
    }
}

void Firm::credit(int amount)
{
    Account::credit(amount);
    std::cout << "Firm credited, balance: " << balance << "\n";
}


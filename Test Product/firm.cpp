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
    // Firm must pay all its employees, hiring and firing as
    // necessary, and then trigger each employee in the resulting
    // list. But first, check for any waiting governmwnt grant.
    
    stats->current->f_start_bal += balance;
    balance += amount_granted;
    stats->current->gov_grant += amount_granted;
    amount_granted = 0;
    
    // Pay existing employees and calculate the total committed wage bill
    int committed = 0;
    for (auto it : employees)
    {
        if (it->isEmployed())   // check that we haven't fired this employee
        {
            stats->current->num_employed += 1;
            
            int wage = it->getWage();
            int dedns = (wage * settings->dedns) / 100;
            if (wage <= balance)
            {
                transferTo(it, wage - dedns);
                transferTo(Government::Instance(), dedns);
                committed += wage;
                stats->current->wages_paid += (wage - dedns);
                stats->current->dedns_paid += dedns;
            }
            else
            {
                // std::cout << "Firm: insufficient funds to pay employee -- firing\n";
                stats->current->num_fired += 1;
                pool->fire(it);
            }
        }
    }

    // Trigger all the employees (note that we pay them all before
    // we trigger any of them. Note that we trigger them even if we
    // no longer employ them, relying on the check on period to
    // prevent double counting
    for (auto it : employees) {
        it->trigger(period);
    }

    // IMPORTANT
    // This has to be recorded after employees are triggered in
    // order to include the effect of sales.

    // Trigger any employees in the pool that have not yet been
    // triggered as they may may some purchases even if currently
    // unemployed. This must be done before we get the final balance.
    // pool->trigger(period);

    stats->current->prod_bal += balance;
    
    // If we have funds left over, hire some more employees
    assert(std_wage>0);
    int num_hires = (((balance * settings->prop_con) / 100) - committed) / std_wage;
    if (num_hires > 0) {
        for (int i = 0; i < num_hires; i++) {
            employees.push_back(pool->hire(std_wage, this));
        }
        stats->current->num_hired += num_hires;
    }
}

void Firm::credit(int amount, bool taxable)
{
    Account::credit(amount, taxable);
    
    if (taxable) {
        int tax = (amount * settings->sales_tax) / 100;
        transferTo(Government::Instance(), tax);
        stats->current->sales_tax_paid += tax;
    }
    
    stats->current->tot_sales += amount;    // gross (incl sales tax)
}

void Firm::grant(int amount)
{
    amount_granted = amount;
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


//
//  firm.cpp
//  Test Product
//
//  Created by David Brown on 06/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

// The 'government-owned' firm is the only firm that is created initially and
// serves as a target for government spending. Other firms come into existence
// depending on economic conditions.
//
// Suppose a firm (or all firms taken together) produce goods and services to the
// value of n cus (currency units). If all their stock is sold (the best case
// scenario) they will receive payments of n cus. This is the most they can pay
// their employees without drawing on credit and is therefore the most they can
// pay out in wages (without drawing on credit). However, their employees will
// have to return some of this money to the government in taxes and will not
// therefore be able to buy all the stock. This means there will be less money
// available to pay out in wages next time, and even less stock will be produced.
// Eventually the company will have to get rid of its employees and go out of
// business.
//
// The only way this can be avoided is by the injection of new money. Mechanisms
// are: constantly increasing credit from the bank, payment of benefits to the
// unemployed (so that in effect the unemployed will be subsidising the employed)
// and direct purchases from the firms by the government (using new money).
//

#include "account.hpp"
#include <cassert>

Firm::Firm(int standard_wage)
{
    std_wage = standard_wage;
}

void Firm::trigger(int period)
{
    // Accoumulate firms' starting balance
    // stats->current->f_start_bal += balance;

    // Check for any waiting government grant.
    balance += amount_granted;
    stats->current->gov_grant += amount_granted;
    amount_granted = 0;
    
    // Pay existing employees, calculating the committed wage bill
    int committed = 0;
    for (auto it : employees)
    {
        if (it->isEmployedBy(this))
        {
            int wage = it->getWage();
            int dedns = (wage * settings->dedns) / 100;
            if (wage <= balance)
            {
                transferTo(it, wage - dedns, this);
                transferTo(Government::Instance(), dedns, this);
                committed += wage;
                stats->current->wages_paid += (wage - dedns);
                stats->current->dedns_paid += dedns;
            }
            else
            {
                // Note that when we fire a worker we don't remove them from the
                // list of employers as deleting from the middle of a vector is
                // inefficient. However, Pool will mark the worker as unemployed
                // by setting employer to nullptr.
                stats->current->num_fired += 1;
                pool->fire(it);
            }
        }
        //it->trigger(period);
    }

    
    // Trigger all the employees -- even if we no longer employ them,
    // relying on checks by the employee to prevent double counting
    for (auto it : employees)
    {
        it->trigger(period);
    }
    

    // If we have funds left over, hire some more employees
    int num_hires = (((balance * settings->prop_con) / 100) - committed) / std_wage;
    if (num_hires > 0)
    {
        for (int i = 0; i < num_hires; i++)
        {
            employees.push_back(pool->hire(std_wage, this));
        }
        stats->current->num_hired += num_hires;
    }
}

void Firm::credit(int amount, Account *creditor)
{
    Account::credit(amount);

    // Base class credits account but doesn't pay tax. We assume seller,
    // not buyer, is responsible for paying sales tax and that payments
    // to a Firm are always for purchases and therefore subject to
    // sales tax.
    int tax = (amount * settings->sales_tax) / 100;
    transferTo(Government::Instance(), tax, this);
    stats->current->sales_tax_paid += tax;
    
    stats->current->tot_sales += amount;    // NB gross amount in stats
}

void Firm::grant(int amount)
{
    amount_granted = amount;
}

size_t Firm::getNumEmployees()
{
    return employees.size();
}

Firm::~Firm()
{
    for (auto it : employees)
    {
        delete it;
        employees.pop_back();
    }
}


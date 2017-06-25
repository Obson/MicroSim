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
    // Accoumulate firms' starting balance
    stats->current->f_start_bal += balance;

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
            stats->current->num_employed += 1;
            
            int wage = it->getWage();
            int dedns = (wage * settings->dedns) / 100;
            if (wage <= balance)
            {
                // transferTo() needs to pass taxable and creditor arguments
                // so they can be picked up and used by credit(), otherwise
                // closing balance isn't updated.
                transferTo(it, wage - dedns, this, true);
                
                // This has the incidental benefit of notifying the government
                // of who has paid (e.g.) taxes. Clearly Government isn't lable
                // for tax, but this is handled by the default value (false),
                // and, redundantly, by the override in the Government class.
                transferTo(Government::Instance(), dedns, this);
                committed += wage;
                stats->current->wages_paid += (wage - dedns);
                stats->current->dedns_paid += dedns;
            }
            else
            {
                stats->current->num_fired += 1;
                pool->fire(it);
            }
        }
    }

    // Trigger all the employees -- even if we no longer employ them,
    // relying on checks by the emloyee to prevent double counting
    for (auto it : employees) {
        it->trigger(period);
    }

    // Closing balance has to be recorded after employees have been
    // triggered in order to include the effect of sales.
    stats->current->prod_bal += balance;
    
    // If we have funds left over, hire some more employees
    int num_hires = (((balance * settings->prop_con) / 100) - committed) / std_wage;
    if (num_hires > 0) {
        for (int i = 0; i < num_hires; i++) {
            employees.push_back(pool->hire(std_wage, this));
        }
        stats->current->num_hired += num_hires;
    }
}

void Firm::credit(int amount, bool taxable, Account *creditor)
{
    // Base class credits account but doesn't pay tax. We
    // assume seller, not buyer, is responsible for paying
    // sales tax. Payments to a Firm are of course (normally,
    // at least) for purchases and therefore subject to sales
    // tax.
    Account::credit(amount, taxable);

    if (taxable) {
        int tax = (amount * settings->sales_tax) / 100;
        transferTo(Government::Instance(), tax, this);
        stats->current->sales_tax_paid += tax;
    }
    
    // Gross amount is recorded in stats
    stats->current->tot_sales += amount;
}

void Firm::grant(int amount)
{
    amount_granted = amount;
}

Firm::~Firm()
{
    // We should free the space taken up by any Workers in the employees
    // vector. But where they have been fired we do not free the space
    // we leave this to be done by Pool.
    for (auto it : employees) {
        delete it;
        employees.pop_back();
    }
}


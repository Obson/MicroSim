//
//  worker.cpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

// ----------------------------------------------------------------------------
// TODO
// We should distinguish between employed and unemployed workers when updating
// the statistics record.
// ----------------------------------------------------------------------------


#include "account.hpp"

Worker::Worker(int wage, Firm *emp)
{
    gov = Government::Instance();
    this->wage = wage;
    employer = emp;
}

int Worker::getWage()
{
    return wage;
}

bool Worker::isEmployed()
{
    return (employer != nullptr);
}

bool Worker::isEmployedBy(Account *emp)
{
    return (emp != nullptr && emp == employer);
}

Firm *Worker::getEmployer()
{
    return employer;
}

void Worker::trigger(int period)
{
    if (period > last_triggered)
    {
        last_triggered = period;

        if (!isEmployed()) {
            // We need to do this here because unemployed workers are
            // not normally credited (may change for unemployment benefit
            // basic income, whatever...)
            stats->current->w_start_bal_unemp += balance;
        }

        // For initial testing (only) we will assume all workers spend the
        // same proportion of their income (Settings::prop_con %). For the
        // sake of simplicity we will also assume that in any given period
        // any given worker spends his/her funds through a single randomly
        // selected firm. As long as there are a lot of workers this
        // should be equivalent to each worker choosing a random selection
        // of firms.
        int purch = (balance * settings->prop_con) / 100;
        transferTo(gov->getRandomFirm(), purch, this, true /* liable for sales tax */);
        if (isEmployed()) {
            stats->current->tot_purchases += purch;
        } else {
            stats->current->tot_purch_unemp += purch;
        }
        
        if (isEmployed()) {
            stats->current->house_bal += balance;
        } else {
            stats->current->w_end_bal_unemp += balance;
        }
    }
}

void Worker::credit(int amount, bool taxable, Account *creditor)
{
    // TO DO:
    // We assume a worker only receives one credit (i.e. wages)
    // per period. This will break down if a worker is paid by
    // more than one employer (or receives benefits), so we need to
    // check not just whether a worker is employed but whether
    // employed by the employer that is doing the triggering. At
    // present we can't do this because the employer is not known,
    // so we should pass the employer's id as an argument...
    
    if (isEmployedBy(creditor))
    {
        // Here we assume that if the creditor is our employer then
        // we should pay income tax. If the creditor isn't our employer
        // but we're receiving the payment in out capacity as Worker
        // then it's probably 'benefits'. This needs to be sorted out
        // properly in due course...
        stats->current->start_bal += balance;
        Account::credit(amount, taxable);
        int tax = (amount * settings->inc_tax) / 100;
        transferTo(gov, tax, this);
        stats->current->wages_recd += amount;
        stats->current->inc_tax_paid += tax;
    }
    else
    {
        // TO DO: We need to flag an alternative source of income for
        // unemployed workers
        // ...
    }
    
}

void Worker::setEmployer(Firm *emp)
{
    employer = emp;
}

void Worker::setWage(int wage)
{
    this->wage = wage;
}

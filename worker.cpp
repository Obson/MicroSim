//
//  worker.cpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

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

Firm *Worker::getEmployer()
{
    return employer;
}

void Worker::trigger(int period)
{
    std::cout << "Worker: triggered with balance " << balance << "\n";
    
    // For initial testing (only) we will assume all workers spend the
    // same proportion of their income (Settings::prop_con %). For the
    // sake of simplicity we will also assume that in any give period
    // any given worker spends his/her funds through a single randomly
    // selected firm. As long as there are a lot of workers this
    // should be equivalent to each worker choosing a random selection
    // of firms.
    int purch = (balance * settings->prop_con) / 100;
    transferTo(gov->getRandomFirm(), purch);
    stats->current->tot_purchases += purch;
    
    stats->current->house_bal += balance;
}

// Note that we assume the only payments to Workers are wages, so we always
// pay income tax. It might be a good idea to make this optional by adding
// an extra parameter (bool taxable = true, for example).
void Worker::credit(int amount)
{
    Account::credit(amount);
    int tax = (amount * settings->inc_tax) / 100;
    transferTo(gov, tax);
    
    stats->current->wages_recd += amount;
    stats->current->inc_tax_paid += tax;
}

void Worker::setEmployer(Firm *emp)
{
    employer = emp;
}

void Worker::setWage(int wage)
{
    this->wage = wage;
}

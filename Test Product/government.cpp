//
//  government.cpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "account.hpp"

Government *Government::_instance = nullptr;
int Government::exp = 0;
int Government::std_wage = 500;

Government *Government::Instance()
{
    if (_instance == nullptr)
    {
        _instance = new Government();
    }
    return _instance;
}

Government::Government()
{
    gov = new Firm(std_wage);
    
    // gov has to be accessible via firms[0], so this must be the first
    // access to the vector and must not be popped until the program
    // terminates,
    firms.push_back(gov);
}

void Government::trigger(int period)
{
    // TO DO
    // We need to create something for the government to spend money on.
    // For now we'll simply send it to the business arm of the government,
    // which in effect stands for nationalised industries.

    
    // IMPORTANT
    // At present this is unconditional, but we need to allow for either
    // automatic (rule-based) or manual changes throughout a run as
    // government expenditure needs to be exogenous.
    //
    if (true /*period < 2*/) {
        transferTo(gov, exp);
    }
    
    gov->trigger(period);
    
}

void Government::setExpenditure(int amount)
{
    exp = amount;
}

void Government::setStandardWage(int amount)
{
    std_wage = amount;
}

//
// This function overrides Account::transferTo to allow a negative balance.
//
void Government::transferTo(Account *recipient, int amount)
{
    recipient->credit(amount);
    balance -= amount;
    stats->current->gov_exp += amount;
    stats->current->gov_bal = balance;
}

// All credits to the Government are (at present) regarded as tax. This
// means that while they offset the deficit we need to keep a separate
// record as well. However we don't distinguish between income tax, sales
// tax, and 'pre-tax deductions'. These are all accounted for elsewhere.
void Government::credit(int amount)
{
    Account::credit(amount);
    stats->current->gov_bal = balance;
    stats->current->tax_recd += amount;
}

Government::~Government()
{
    while (!firms.empty())
    {
        delete firms.back();
        firms.pop_back();
    }
}

Firm *Government::getRandomFirm()
{
    return firms[std::rand() % firms.size()];
}

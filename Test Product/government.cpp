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
    std::cout << "Government triggered\n";
    
    // TO DO
    // We need to create something for the government to spend money on.
    // For now we'll simply send it to the business arm of the government,
    // which in effect stands for nationalised industries.
    //
    // IMPORTANT
    // At present the only firm in existence is the government-owned firm
    // (gov). If we keep on spending money into this firm it will keep
    // expanding. This will provide us with a means of expanding the economy,
    // but as we don't want it to be a single-industry economy we won't spend
    // any more on it after the zeroth period. This will change eventually.
    //
    if (period < 2) {
        std::cout << "Government spending " << exp << " units\n";
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
    
    std::cout << "Government balance is now " << balance << "\n";
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

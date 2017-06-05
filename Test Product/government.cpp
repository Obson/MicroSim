//
//  government.cpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "account.hpp"

Government *Government::_instance = 0;

Government *Government::Instance()
{
    if (_instance == 0)
    {
        _instance = new Government();
    }
    return _instance;
}

Government::Government()
{
    exp = 0;
}

void Government::trigger(int period)
{
    std::cout << "Government triggered\n";
    
    std::cout << "Spending " << exp << " units\n";

    // TO DO
    // We need to create something for the government to spend money on.
    // ...
}

void Government::setExpenditure(int amount)
{
    exp = amount;
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

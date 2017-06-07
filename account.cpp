//
//  account.cpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "account.hpp"

Account::Account()
{
    balance = 0;
}

int Account::getBalance()
{
    return balance;
}

bool Account::transferTo(Account *recipient, int amount)
{
    if (amount > balance) {
        std::cout << "Account: insufficient funds\n";
        return false;
    } else {
        recipient->credit(amount);
        balance -= amount;
        return true;
    }
}

void Account::credit(int amount)
{
    balance += amount;
}


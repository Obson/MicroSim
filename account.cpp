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
    stats = Statistics::Instance();
    settings = Settings::Instance();
    balance = 0;
    
    id = Settings::getId();
}

int Account::getBalance()
{
    return balance;
}

// Use transferTo() in preference to credit()as credit() doesn't upate our balance 
bool Account::transferTo(Account *recipient, int amount, Account *creditor)
{
    if (amount > balance) {
        std::cout << "Account " << id << ": insufficient funds (" << balance << " available, " << amount << " to pay)\n";
        return false;
    } else {
        recipient->credit(amount, creditor);
        balance -= amount;
        return true;
    }
}

void Account::credit(int amount, Account *creditor)
{
    balance += amount;
}

int Account::getId() const
{
    return id;
}

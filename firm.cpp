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
    
    init();
}

void Firm::init()
{
    wages_paid = 0;
    sales_tax_paid = 0;
    sales_receipts = 0;
    dedns_paid = 0;
    num_hired = 0;
    num_fired = 0;

    balance += amount_granted;
}

void Firm::trigger(int period)
{
    init();
    
    // Pay existing employees, calculating the committed wage bill
    int committed = 0;
    for (auto it : employees)
    {
        if (it->isEmployedBy(this))
        {
            int wage = it->getWage();
            int dedns = (wage * settings->getPreTaxDedns()) / 100;
            
            it->init();
            
            if (wage <= balance)
            {
                transferTo(it, wage - dedns, this);
                transferTo(Government::Instance(), dedns, this);
                committed += wage;
                wages_paid += wage - dedns;
                dedns_paid += dedns;
            }
            else
            {
                // Note that when we fire a worker we don't remove them from the
                // list of employers as deleting from the middle of a vector is
                // inefficient.
                // stats->current->num_fired += 1;
                num_fired += 1;
                Government::Instance()->fire(it);
            }
        }
    }

    
    // Trigger all the employees -- even if we no longer employ them,
    // relying on checks by the employee to prevent double counting.
    //
    // TO DO: This should probably be incorporated into the previous loop
    // but check that this works first...
    for (auto it : employees)
    {
        it->trigger(period);
    }
    

    // If we have funds left over, hire some more employees
    int n = (((balance * settings->getPropCon()) / 100) - committed) / std_wage;
    if (n > 0)
    {
        for (int i = 0; i < n; i++)
        {
            employees.push_back(Government::Instance()->hire(std_wage, this));
        }
        num_hired = n;
    }
}

void Firm::credit(int amount, Account *creditor)
{
    Account::credit(amount);
    
    sales_receipts += amount;

    // Base class credits account but doesn't pay tax. We assume seller,
    // not buyer, is responsible for paying sales tax and that payments
    // to a Firm are always for purchases and therefore subject to
    // sales tax.
    int r = settings->getSalesTaxRate();    // e.g. 25%
    int r_eff = (100 * r) / (100 + r);      // e.g. 20%
    int t = (amount * r_eff) / 100;
    if (transferTo(Government::Instance(), t, this))
    {
        sales_tax_paid += t;
    }
}

void Firm::grant(int amount)
{
    amount_granted = amount;
}

int Firm::getAmountGranted()
{
    return amount_granted;
}

int Firm::getWagesPaid()
{
    return wages_paid;
}

int Firm::getSalesTaxPaid()
{
    return sales_tax_paid;
}

int Firm::getSalesReceipts()
{
    return sales_receipts;
}

int Firm::getDednsPaid()
{
    return dedns_paid;
}

size_t Firm::getNumEmployees()
{
    return employees.size();
}

int Firm::getNumHired()
{
    return num_hired;
}

int Firm::getNumFired()
{
    return num_fired;
}

int Firm::getIncTaxPaid()
{
    int bal = 0;
    for (auto it : employees)
    {
        bal += it->getIncTaxPaid();
    }
    return bal;
}

int Firm::getTotEmpBal()
{
    int bal = 0;
    for (auto it : employees)
    {
        if (it->getEmployer() == this) {
            bal += it->getBalance();
        }
    }
    return bal;
}

int Firm::getEmpPurch()
{
    int bal = 0;
    for (auto it : employees)
    {
        if (it->getEmployer() == this) {
            bal += it->getPurchasesMade();
        }
        assert(bal<1000000);
    }
    return bal;
}

int Firm::getUnempPurch()
{
    int bal = 0;
    for (auto it : employees)
    {
        if (it->getEmployer() == nullptr) {
            bal += it->getPurchasesMade();
        }
    }
    return bal;
}

int Firm::getBenefitsRecd()
{
    int bal = 0;
    for (auto it : employees)
    {
        if (it->getEmployer() == nullptr) {
            bal += it->getBenefitsReceived();
        }
    }
    return bal;
}

Firm::~Firm()
{
    for (auto it : employees)
    {
        delete it;
        employees.pop_back();
    }
}


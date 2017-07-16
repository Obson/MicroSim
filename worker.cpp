//
//  worker.cpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "account.hpp"

Worker::Worker()
{
    gov = Government::Instance();
    employer = nullptr;
    period_hired = -1;
    period_fired = 0;
    init();
}

void Worker::init()
{
    wages = 0;
    benefits = 0;
    purchases = 0;
    inc_tax = 0;
}

bool Worker::isEmployed()
{
    return (employer != nullptr);
}

bool Worker::isEmployedBy(Account *emp)
{
    return (emp != nullptr && emp == employer);
}

void Worker::setPeriodHired(int period)
{
    period_hired = period;
}

bool Worker::isNewHire(int period)
{
    return period_hired == period;
}

Firm *Worker::getEmployer()
{
    return employer;
}

void Worker::trigger(int period)
{
    if (period > last_triggered)    // to prevent double counting
    {
        last_triggered = period;
        int purch = (balance * settings->getPropCon()) / 100;
        if (purch > 0 && transferTo(reg->selectRandomFirm(), purch, this))
        {
            purchases += purch;
        }
    }
}

void Worker::credit(int amount, Account *creditor)
{
    Account::credit(amount);

    if (isEmployedBy(creditor))
    {
        // Here we assume that if the creditor is our employer then we should
        // pay income tax. If the creditor isn't our employer but we're
        // receiving the payment in our capacity as Worker then it's probably
        // benefits or bonus. If not employed at all it must be bonus and we are
        // flagged for deletion. Surprising but perfectly possible.
        int tax = (amount * settings->getIncTaxRate()) / 100;
        transferTo(gov, tax, this);
        wages += amount;
        inc_tax += tax;
    }
    else if (creditor == gov)
    {
        benefits += amount;
    }
    else
    {
        std::cout << "*** Unknown reason for credit ***\n";
        exit(100);
    }
}

int Worker::getWagesReceived()
{
    return wages;
}

int Worker::getBenefitsReceived()
{
    return benefits;
}

int Worker::getPurchasesMade()
{
    return purchases;
}

void Worker::setEmployer(Firm *emp)
{
    employer = emp;
}

int Worker::getIncTaxPaid()
{
    return inc_tax;
}


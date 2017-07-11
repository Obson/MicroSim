//
//  register.cpp
//  microsim
//
//  Created by David Brown on 09/07/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include <assert.h>
#include "register.hpp"
#include "account.hpp"

Register *Register::_instance = nullptr;

Register *Register::Instance()
{
    if (_instance == nullptr) {
        _instance = new Register();
    }
    return _instance;
}

Register::Register()
{
    num_hired = 0;
    num_fired = 0;
    num_just_fired = 0;         // used by payWorkers() only
}

Register::~Register()
{
    for (auto it : firms) {
        delete it;
    }
    for (auto it : workers) {
        delete it;
    }
}

Firm *Register::createFirm()
{
    Firm *firm = new Firm();
    firms.push_back(firm);
    
    return firm;
}

Firm *Register::selectRandomFirm()
{
    return firms[std::rand() % firms.size()];
}

size_t Register::getNumFirms()
{
    return firms.size();
}

int Register::getProdBal()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getBalance();
    }
    
    return bal;
}

int Register::getGrantsRecd()
{
    int tot = 0;
    for (auto it : firms)
    {
        tot += it->getAmountGranted();
    }
    
    return tot;
}

int Register::getDednsPaid()
{
    int tot = 0;
    for (auto it : firms) {
        tot += it->getDednsPaid();
    }
    
    return tot;
}

int Register::getWagesPaid()
{
    int tot = 0;
    for (auto it : firms) {
        tot += it->getWagesPaid();
    }
    
    return tot;
}

int Register::getBonusesPaid()
{
    int tot = 0;
    for (auto it : firms) {
        tot += it->getBonusesPaid();
    }
    
    return tot;
}

int Register::getSalesReceipts()
{
    int tot = 0;
    for (auto it : firms) {
        tot += it->getSalesReceipts();
    }
    
    return tot;
}

int Register::getSalesTaxPaid()
{
    int tot = 0;
    for (auto it : firms) {
        tot += it->getSalesTaxPaid();
    }
    
    return tot;
}

size_t Register::getNumWorkers()
{
    return workers.size();
}

int Register::payWorkers(int amount, int max_tot, Account *source, Reason reason, int period)
{
    int amt_paid = 0;
    if (reason == wages) {
        num_just_fired = 0;  // reset - will be incremented by fired()
    }
    
    if (!workers.empty()) {
        for (auto it : workers) {
            switch (reason) {
                case wages:
                    if (it->getEmployer() == source) {
                        if (max_tot - amt_paid < amount) {
                            fire(it, period);
                        } else {
                            it->credit(amount, source);
                            amt_paid += amount;
                        }
                    }
                    break;
                case benefits:
                    if (!it->isEmployed()) {
                        it->credit(amount, source);
                        amt_paid += amount;
                    }
                    break;
                case bonus:
                    // Note that when paying bonuses we do not check that
                    // sufficient funds are available -- it's up to the caller to
                    // ensure that the amount is correct. Any overpayment will
                    // simply create a negative balance in the caller's account.
                    if (it->getEmployer() == source) {
                        it->credit(amount, source);
                        amt_paid += amount;
                    }
                    break;
            }
        }
    }
    
    return amt_paid;
}

// getNumJustFired() gets the number of workers that were fired the last time
// payWorkers() was called with reason == wages in the current period. It's
// provided because the value returned is the amount paid, but the caller may
// also want to know how many workers were fired. For the total number of
// workers fired this period use getNumFired() instead.

int Register::getNumJustFired()
{
    return num_just_fired;
}

int Register::getNumHired()
{
    return num_hired;
}

int Register::getNumFired()
{
    return num_fired;
}

int Register::getWorkersBal(Register::Status status)
{
    int tot = 0;
    for (auto it : workers) {
        switch (status)
        {
            case any:
                tot += it->getBalance();
                break;
            case employed:
                if (it->isEmployed()) {
                    tot += it->getBalance();
                }
                break;
            case unemployed:
                if (!it->isEmployed()) {
                    tot += it->getBalance();
                }
                break;
        }
    }
    
    return tot;
}

int Register::getNumEmployed()
{
    int n = 0;
    for (auto it : workers) {
        if (it->isEmployed()) {
            n++;
        }
    }
    
    return n;
}

int Register::getNumEmployedBy(Firm *firm)
{
    int n = 0;
    for (auto it : workers) {
        if (it->isEmployedBy(firm)) {
            n++;
        }
    }
    
    return n;
}

int Register::getNumUnemployed()
{
    int n = 0;
    for (auto it : workers) {
        if (!it->isEmployed()) {
            n++;
        }
    }
    
    return n;
}

int Register::getIncTaxPaid()
{
    int tot = 0;
    for (auto it : workers) {
        tot += it->getIncTaxPaid();
    }
    
    return tot;
}

int Register::getPurchasesMade()
{
    int tot = 0;
    for (auto it : workers) {
        tot += it->getPurchasesMade();
    }
    
    return tot;
}

int Register::getWagesRecd()
{
    int tot = 0;
    for (auto it : workers) {
        tot += it->getWagesReceived();
    }
    
    return tot;
}

int Register::getBenefitsRecd()
{
    int tot = 0;
    for (auto it : workers) {
        tot += it->getBenefitsReceived();
    }
    
    return tot;
}

Worker *Register::hire(Firm *employer, int period)
{
    Worker *w = nullptr;
    for (auto it : workers) {
        if (it->getEmployer() == nullptr) {
            w = it;
        }
    }
    if (w == nullptr) {
        w = new Worker();
        workers.push_back(w);
    }
    
    w->employer = employer;
    w->period_hired = period;
    
    num_hired += 1;
    
    return w;
}

int Register::hireSome(Firm *employer, int period, int n)
{
    int count;
    for (count = 0; count < n; count++) {
        if (hire(employer, period) == nullptr) {
            break;
        }
    }
    
    num_hired += count;
    return count;
}

void Register::fire(Worker *w, int period)
{
    w->employer = nullptr;
    w->period_fired = period;
    num_fired++;
}

void Register::init()
{
    for (auto it : firms) {
        it->init();
    }
    for (auto it : workers) {
        it->init();
    }
    
    num_hired = 0;
    num_fired = 0;
    num_just_fired = 0;
}

void Register::trigger(int period)
{
    for (auto it : firms) {
        it->trigger(period);
    }
    for (auto it : workers) {
        it->trigger(period);
    }
    
    for (auto it : firms) {
        it->epilogue(period);
    }
}

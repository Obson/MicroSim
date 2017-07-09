//
//  government.cpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "account.hpp"

Government *Government::_instance = nullptr;

Government *Government::Instance()
{
    if (_instance == nullptr) {
        _instance = new Government();
    }
    return _instance;
}

void Government::init()
{
    exp = 0;
    rec = 0;
    ben = 0;
}

Government::Government()
{
    firms.insert(&gov);
}

// TO DO NEXT. OR AT LEAST VERY SOON
// ---------------------------------
// This method creates and registers a new independent firm with, at present,
// no visible means of support. To fix this we will need to provide a means
// of accessing government funds. Conventionally this is achieved via banks,
// so what we probably need to do is implement the Bank class and set the
// new firm up with a 'line of credit'. I think this means that the firm can
// keep borrowing more money as long as it keeps up the repayments on the
// existing loan and pays the interest due. I suspect this is inherently
// unstable and is only sustainable as long as the firm continues to grow.
// Eventually the market becomes saturated and the firm dies -- or possibly
// it just sucks in business from other less successful firms in the same
// field, and they die.

Firm *Government::createFirm()
{
    Firm *firm = new Firm(settings->getStdWage());
    firms.insert(firm);
    return firm;
}

int Government::getExpenditure()
{
    return exp;
}

int Government::getBenefitsPaid()
{
    return ben;
}

int Government::getReceipts()
{
    return rec;
}

size_t Government::getNumFirms()
{
    return firms.size();
}

size_t Government::getNumEmployees()
{
    size_t res = 0;
    for (auto it : firms) {
        res += it->getNumEmployees();
    }
    return res;
}

int Government::getNumHired()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getNumHired();
    }
    return bal;
}

int Government::getNumFired()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getNumFired();
    }
    return bal;
}

int Government::getProdBalance()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getBalance();
    }
    return bal;
}

int Government::getUnempBal()
{
    int bal = 0;
    for (auto it : available) {
        bal += it->getBalance();
    }
    return bal;
}

int Government::getEmpBal()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getTotEmpBal();
    }
    for (auto it : available) {
        bal += it->getBalance();
    }
    return bal;
}

int Government::getGrantsRecd()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getAmountGranted();
    }
    return bal;
}

int Government::getDednsPaid()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getDednsPaid();
    }
    return bal;
}

int Government::getWagesPaid()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getWagesPaid();
    }
    return bal;
}

int Government::getBonusesPaid()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getBonusesPaid();
    }
    return bal;
}

int Government::getPurchases()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getSalesReceipts();
    }
    return bal;
}

int Government::getIncTaxPaid()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getIncTaxPaid();
    }
    for (auto it : available) {
        bal += it->getIncTaxPaid();
    }
    return bal;
}

int Government::getSalesTaxPaid()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getSalesTaxPaid();
    }
    return bal;
}

int Government::getEmpPurch()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getEmpPurch();
    }
    for (auto it : available) {
        bal += it->getPurchasesMade();
    }
    return bal;
}

int Government::getBenefitsRecd()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getBenefitsRecd();
    }
    for (auto it : available) {
        bal += it->getBenefitsReceived();
    }
    return bal;
}

int Government::getWagesRecd()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getWagesRecd();
    }
    for (auto it : available) {
        bal += it->getWagesReceived();
    }
    return bal;
}

void Government::trigger(int period)
{
    //std::cout << "\nGovernment::trigger(" << period << ")\n";
    
    init(); // zero all the per-period accumulators
    
    int amt = settings->getGovExpRate();
    gov.grant(amt);
    balance -= amt;
    exp += amt;
    
    // Ensure all firms are initialised before we trigger any of them. I'm
    // not exactly sure why this is necessary but it is.
    for (auto it : firms) {
        it->init();
    }
    for (auto it : available) {
        it->init();
    }
    
    for (auto it : firms) {
        it->trigger(period);
    }
    for (auto it : available) {
        it->trigger(period);
    }

    // Note that we only pay unemployment benefit to unemployed workers,
    // i.e. workers who have been employed and are no longer. Initially
    // we assume an economically inactive population that is very much
    // larger than the number that have been or are employed. As the
    // economy matures this disparity will diminish.
    int benefit_amount = (settings->getStdWage() * settings->getUBR()) / 100;
    for (auto it : available) {
        transferTo(it, benefit_amount, this);
        ben += benefit_amount;
    }
}

//
// This function overrides Account::transferTo to allow a negative balance.
//
void Government::transferTo(Account *recipient, int amount, Account *creditor)
{
    // We adopt the convention that receipts from the government are not
    // taxable. This is probably a rather murky area, given that the
    // mechanisms by which government injects money into the economy are
    // unclear and seem to involve financing banks to make more loans.
    // In the case of the NHS, nationalised industries, the civil service
    // and the 'armed forces' the mechanism is probably more direct.
    // Anyway, to go into this would be a distraction so we'll simply
    // treat it as untaxable payment for services.
    recipient->credit(amount, this);
    balance -= amount;
}

// All credits to the Government are (at present) regarded as tax. This
// means that while they offset the deficit we need to keep a separate
// record as well. However we don't distinguish between income tax, sales
// tax, and 'pre-tax deductions'. These are all accounted for elsewhere.
// Obviously, the government doesn't pay tax.
void Government::credit(int amount, Account *creditor)
{
    Account::credit(amount);
    rec += amount;
}

Worker *Government::hire(int wage, Firm *emp, int period)
{
    if (available.empty()) {
        return getNumEmployees() < settings->getPopSize()
        ? new Worker(wage, emp, period)
        : nullptr;
    } else {
        auto it = available.begin();
        advance(it, std::rand() % available.size());
        Worker *w = *it;
        w->setPeriodHired(period);
        available.erase(it);
        return w;
    }
}

// This function records the fired worker in the 'available' map, but it's
// the caller's responsibility to remove if from the 'firms' map.
//void Government::fire(std::map<int, Worker>::const_iterator w)
void Government::fire(Worker *w)
{
    w->setEmployer(nullptr);
    available.insert(w);
}

Firm *Government::getRandomFirm()
{
    std::set<Firm*>::const_iterator it(firms.begin());
    advance(it, std::rand() % firms.size());
    return *it;
}

Government::~Government()
{
    for (auto it : firms)
    {
        if (it != &gov) {
            delete it;
        }
    }
    for (auto it : available)
    {
        delete it;
    }
}



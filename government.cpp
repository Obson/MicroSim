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
}

Government::Government()
{
    gov = new Firm(settings->getStdWage());
    
    // gov has to be accessible via firms[0], so this must be the first
    // access to the vector and must not be popped until the program
    // terminates,
    firms.push_back(gov);
}

// TO DO NEXT
// ----------
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
    firms.push_back(firm);
    return firm;
}

int Government::getExpenditure()
{
    return exp;
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

int Government::getPurchases()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getSalesReceipts();
        //std::cout << "Government[" << id << "]::getPurchases(): bal = " << bal << "\n";
    }
    return bal;
}

int Government::getIncTaxPaid()
{
    int bal = 0;
    for (auto it : firms) {
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
    return bal;
}

int Government::getUnempPurch()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getUnempPurch();
    }
    return bal;
}

int Government::getBenefitsRecd()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getBenefitsRecd();
    }
    return bal;
}

void Government::trigger(int period)
{
    //std::cout << "\nGovernment::trigger(" << period << ")\n";
    
    init(); // zero all the per-period accumulators
    
    // IMPORTANT
    // At present this is unconditional, but we need to allow for either
    // automatic (rule-based) or manual changes throughout a run as
    // government expenditure needs to be exogenous.
    //   Note also that we use a special 'grant' method to transfer
    // government funds, to avoid disrupting the standard payment
    // mechanism. This also ensures that no tax will be paid by recipients

    if (true /*period < 2*/) {
        int amt = settings->getGovExpRate();
        gov->grant(amt);
        balance -= amt;
        exp += amt;
    }
    
    // Ensure all firms are initialised before we trigger any of them. I'm
    // not exactly sure why this is necessary but it is.
    for (auto it : firms) {
        it->init();
    }
    for (auto it : firms) {
        it->trigger(period);
    }
    
    for (auto it : available) {
        it->trigger(period);
    }

    // Note that we only pay unemployment
    // benefit to unemployed workers, i.e. workers who have been employed
    // and are no longer. Initially we assume an economically inactive
    // population that is very much larger than the number that have been
    // or are employed. As the economy matures this disparity will diminish
    // and we can perhaps pay benefit to the economically inactive as well.
    
    int benefit_amount = (settings->getStdWage() * settings->getUBR()) / 100;
    for (auto it : available) {
        transferTo(it, benefit_amount, this);
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

Worker *Government::hire(int wage, Firm *emp)
{
    return getNumEmployees() < settings->getPopSize()
        ? new Worker(wage, emp)
        : nullptr;
    
    // TO DO
    //
    // Re-hire existing worker if possible; otherwise get a new
    // one. This is something that could be refined to take into
    // account the population size, previous wage of existing
    // workers, etc. May need to rethink storage -- perhaps use
    // std::set instea of std::vector so we don't get duplicates.
    //
    
/*
    Worker *w;
    
    if (!available.empty()) {
        // Re-hire fired worker
        w = available.back();
        w->setWage(wage);
        w->setEmployer(emp);
        available.pop_back();
    } else {
        
        // Hire new worker
        w = new Worker(wage, emp);
        
    }
    
    return w;
*/
}

void Government::fire(Worker *w)
{
    w->setEmployer(nullptr);
    available.push_back(w);
}

Government::~Government()
{
    while (!firms.empty()) {
        delete firms.back();
        firms.pop_back();
    }
 
    // Firms will have deleted all the 'available' employees as
    // they will still be in their 'employees' vector. So the following
    // loop is redundant...
    /*
    while (!available.empty()) {
        delete available.back();
        available.pop_back();
    }
    */
}

Firm *Government::getRandomFirm()
{
    return firms[std::rand() % firms.size()];
}

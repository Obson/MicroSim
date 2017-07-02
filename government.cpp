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

int Government::getProdBalance()
{
    int bal = 0;
    for (auto it : firms) {
        bal += it->getBalance();
    }
    return bal;
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
    //   Note also that we use a special 'grant' method to transfer
    // government funds, to avoid disrupting the standard payment
    // mechanism. This also ensures that no tax will be paid by recipients
    //
    if (true /*period < 2*/) {
        int exp = settings->getGovExpRate();
        gov->grant(exp);
        balance -= exp;
        stats->current->gov_exp += exp;
        stats->current->gov_bal = balance;  // Gov sector balance
    }
    
    // TO DO: Incorporate this into previous loop and test
    for (auto it : firms) {
        it->trigger(period);
    }
    
    for (auto it : available) {
        it->trigger(period);
    }


    // TO DO: Pay unemployment benefit. Note that we only pay unemployment
    // benefit to unemployed workers, i.e. people who have been employed.
    // This is because initially we assume an economically inactive
    // population that is very much larger than the number that have been
    // or are employed. As the economy matures this disparity will diminish
    // and we can perhaps pay benefit to the economically inactive as well.
    // ...
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
    stats->current->gov_exp += amount;
    stats->current->gov_bal = balance;
}

// All credits to the Government are (at present) regarded as tax. This
// means that while they offset the deficit we need to keep a separate
// record as well. However we don't distinguish between income tax, sales
// tax, and 'pre-tax deductions'. These are all accounted for elsewhere.
// Obviously, the government doesn't pay tax.
void Government::credit(int amount, Account *creditor)
{
    Account::credit(amount);
    stats->current->gov_bal = balance;
    stats->current->tax_recd += amount;
}

Worker *Government::hire(int wage, Firm *emp)
{
    // At present we don't check the number of workers so we
    // effectively allow an infinite number. Ideally we should check
    // how close to count we are and possibly recruit an existing
    // worker at a higher wage. The point at which we start doing
    // this will be one of the main determinants of inflation.
    
    Worker *w;
    
    // Re-hire existing worker if possible; otherwise get a new
    // one. This is something that could be refined to take into
    // account the population size, previous wage of existing
    // workers, etc.
    //
    // BUG: Rehiring causes reconciliations to fail because (I think)
    // we reassign an employer and then make it available to the calling
    // Firm. If re-hired we will now have a duplicate in the employees
    // vector (or in the vectors held by different firms). In the end
    // we're going to have to remove fired Workers from the employees
    // vector, regardless of its inefficiency. It might be worth
    // considering using a set instead of a vector.
    
    /*
     if (!available.empty()) {
     // Re-hire fired worker
     w = available.back();
     w->setWage(wage);
     w->setEmployer(emp);
     available.pop_back();
     } else {
     */
    
    // Hire new worker
    w = new Worker(wage, emp);
    
    //}
    
    return w;
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
    
    while (!available.empty()) {
        delete available.back();
        available.pop_back();
    }
}

Firm *Government::getRandomFirm()
{
    return firms[std::rand() % firms.size()];
}

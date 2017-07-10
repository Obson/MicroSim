//
//  account.hpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#ifndef account_hpp
#define account_hpp

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <list>

#include "settings.hpp"
#include "statistics.hpp"
#include "register.hpp"

// Account is the base class for every active entity in the system
// and provides the basic overrideable functionality.

class Statistics;
class Settings;
class Firm;
class Government;

class Account
{
public:

    Account();
    
    virtual int getBalance();
    
    // This function is declared as virtual to allow derived class
    // to add functionality, e.g. diagnostics
    virtual void credit(int amount, Account *creditor = nullptr);
    
    // Every derived class must provide a trigger function,
    // which will be called once per period.
    virtual void trigger(int period) = 0;
    
    int getId() const;
    
protected:
    
    Register *reg;
    Settings *settings;
    Statistics *stats;
    Government *gov;

    int balance;
    int last_triggered = -1;

    bool transferTo(Account *recipient, int amount, Account *creditor);

private:

    int id;
    
};


// The Firm class is used in Worker, which it also uses, so we
// need a forward declaration here.

//class Register;

class Worker: public Account
{
    friend class Government;
    friend class Firm;
    friend class Register;
    
private:

    int wage;
    Firm *employer;

    // Receipts and payments for current period only. Must be
    // set to zero on each trigger (unless re-triggered in same
    // period) and accumulated throughout period. 'Get' methods
    // must be used to retrieve these values at the end of each
    // period for statistics.
    int purchases;
    int benefits;
    int wages;
    int inc_tax;
    int period_hired;
    int period_fired;
    
protected:
    
    void init();
    
    void setEmployer(Firm*);
    void setWage(int);
    void setPeriodHired(int period);
    
public:
    
    Worker(int wage, Firm *emp, int period);
    Worker();
    
    int getWage();
    Firm *getEmployer();
    bool isEmployed();
    bool isEmployedBy(Account *emp);
    bool isNewHire(int period);

    // Overrides
    void credit(int amount, Account *creditor = nullptr);
    void trigger(int period);

    int getWagesReceived();
    int getBenefitsReceived();
    int getPurchasesMade();
    int getIncTaxPaid();
};

class Firm: public Account
{
    friend class Government;
    friend class Register;
    
private:
    
    int std_wage;
    int amount_granted = 0;
    int wages_paid = 0;
    int bonuses_paid = 0;
    int sales_tax_paid = 0;
    int sales_receipts = 0;
    int dedns_paid = 0;
    int num_hired = 0;
    int num_fired = 0;
    
    int committed = 0;              // current cost of wages and deductions
    
protected:

    // Only government can make a grant, so this should be protected
    // and the Government class made a friend class
    void grant(int amount);
    
    void init();
    
public:
    
    // RATIONALE
    // ---------
    // We consider a year to be made up of 50 periods and a
    // typical yearly wage to be 25000 -- i.e. 500 per period.
    // Initially we will assume everyone is paid the same
    // amount, but we will need to allow different schemes,
    // such as random with even distribution, random with
    // normal distribution, and random with lognormal
    // distribution. The latter seems to give a fairly
    // convincing 'long-tail' distribution.
    //
    // A distribution based on recorded peercentile
    // figures for the UK in 2015 approximates to the
    // polynomial:
    //
    // 0.00001229(x^6) + 0.0033(x^5) + 0.3373(x^4) + 16.066(x^3) + 359
    //
    // where 0 < x < 100 is the percentile. It should be possible
    // to work out a distribution function that would produce
    // this result. The table itself can be found at
    //
    // https://www.gov.uk/government/statistics/percentile-points-from-1-to-99-for-total-income-before-and-after-tax
    //
    // and it might be more straightforward to base wages on
    // that. An alternative approach would be to start off with
    // equal (or evenly distributed) wages and see what
    // develops over time.
    
    Firm(int standard_wage = 500);
    
    void trigger(int period);
    void epilogue(int period);
    
    // Overrides base mmethod to give additional functionality
    void credit(int amount, Account *creditor = nullptr);
    
    int getAmountGranted();
    int getWagesPaid();
    int getBonusesPaid();
    int getSalesTaxPaid();
    int getSalesReceipts();
    int getDednsPaid();
    
    size_t getNumEmployees();
    
    int getNumHired();
    int getNumFired();
};

// Government is a singleton class as we are currently assuming
// a closed economy wth a single currency. Foreign trade would
// require firms that were registered to a different Government.
// This should be added later.

class Government: public Account
{
private:
    
    static Government *_instance;
    
    Firm *gov;  // (see constructor for assignment to firms)
    Register *reg;
    
    int exp, rec, ben;
    
protected:
    
    Government();
    
    void init();
    
    // This method overrides the method in the base (Account) class, which
    // prohibits transfers that would leave a negative balance. This
    // restriction doesn't apply to the government, which creates money
    // precisely by creating transfers that leave a negative balance.
    void transferTo(Account *recipient,
                    int amount,
                    Account *creditor
                    );
    
    // We override the base method here just so we can extract
    // the balance for statistics.
    void credit(int amount, Account *creditor = nullptr);
    
public:
    
    static Government *Instance();
    
    void trigger(int period);
    
    int getExpenditure();   // Gov expenditure in current period (excl benefits)
    int getBenefitsPaid();  // Benefits paid this period
    int getReceipts();      // Gov receipts (taxes and dedns) in current period
};


class Bank: public Account
{
    
};



#endif /* account_hpp */

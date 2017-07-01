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
#include <vector>

#include "settings.hpp"
#include "statistics.hpp"

// Account is the base class for every active entity in the system
// and provides the basic overrideable functionality.

class Statistics;
class Settings;

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
    
protected:
    
    Settings *settings;
    Statistics *stats;

    int balance;
    int last_triggered = -1;

    bool transferTo(Account *recipient, int amount, Account *creditor);
    
};


// The Firm class is used in Worker, which it also uses, so we
// need a forward declaration here.

class Firm;
class Government;

// A Worker instance represents an economically active individual.
// 'Economically active' means (a) having an employer (which may
// be null) and (b) having a wage level (which may be zero).
// Workers (who are also consumers -- the terms are effectively
// interchangeble( are managed by the singleton Pool instance.

class Worker: public Account
{
    friend class Pool;
    
private:

    int wage;
    Firm *employer;
    Government *gov;

protected:
    
    void setEmployer(Firm*);
    void setWage(int);
    
public:
    
    Worker(int wage, Firm *emp);
    
    int getWage();
    Firm *getEmployer();
    bool isEmployed();
    bool isEmployedBy(Account *emp);

    // Overrides
    void credit(int amount, Account *creditor = nullptr);
    void trigger(int period);
};


// Pool is a singleton class acting as a factory for workers.
// It also 'recycles' them when they are fired, but details
// have yet to be thought out.

class Pool
{
private:
    
    std::vector<Worker*> available;
    
    static Pool *_instance;
    
    static int _count;
    
protected:
    
    Pool();
    
public:
    
    static Pool *Instance();
    static void setCount(int count);
    
    ~Pool();
    
    void trigger(int period);
    
    Worker *hire(int wage, Firm *emp);
    void fire(Worker*);
    
};


// A Firm instance is associated with a set of Workers, referred
// to as 'employees'. The Firm is their 'employer'. A Firm has
// a responsibility to pay each of its employees that employee's
// agreed wage on each period. Employees are hired by making a
// request to the Pool and fired by returning them to the Pool.

class Firm: public Account
{
private:
    
    std::vector<Worker *> employees;
    
    Pool *pool = Pool::Instance();
    
    int std_wage;
    int amount_granted = 0;
    
protected:
    
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
    
    ~Firm();    // free space taken up by employees list in vector
    
    void trigger(int period);
    
    // Overrides base mmethod to give additional functionality
    void credit(int amount, Account *creditor = nullptr);
    
    void grant(int amount);
    
    size_t getNumEmployees();
};


// Government is a singleton class as we are currently assuming
// a closed economy wth a single currency. Foreign trade would
// require firms that were registered to a different Government.
// This should be added later.

class Government: public Account
{
private:
    
    static Government *_instance;
    
    // Firms in a capitalist economy are legal entities and
    // must be registered with the government. The zeroth
    // element of this vector (firms[0]) represents the
    // 'business arm' of the government (in other words the
    // nationalised industries) and is treated as a special
    // case
    std::vector<Firm*> firms;
    
    Firm *gov;  // (see constructor for assignment to firms)
    
    // Probably a temporary measure, exp is the amount the
    // government spends per period. It may be better to make
    // it a variable (exogenous) parameter of the system or
    // to determine it algorithmically in some way.
    static int exp, std_wage;
    
protected:
    
    Government();
    
    // This method overrides the method in the base (Account)
    // class, which prohibits transfers that would leave a
    // negative balance. This restriction doesn't apply to
    // the government, which creates money precisely by
    // creating transfers that leave a negative balance.
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
    
    Firm *createFirm();

    Firm *getRandomFirm();
    
    size_t getNumFirms();
    
    // Note that this returns the total number of employed workers, not
    // just those employed by the government. For that you would need
    // to access Government::gov->getNumFirms(), but you can't at prsent
    // because it's a private method.
    size_t getNumEmployees();
    
    // This function returns the total balance of all the firms that have
    // been registered, including gov.
    int getProdBalance();
    
    ~Government();
};


class Bank: public Account
{
    
};



#endif /* account_hpp */

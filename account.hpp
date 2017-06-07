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

// Account is the base class for every active entity in the system
// and provides the basic overrideable functionality.

class Account
{
public:

    Account();
    int getBalance();
    
    // This function is declared as virtual to allow derived class
    // to add functionality, e.g. diagnostics
    virtual void credit(int amount);
    
    // Every derived class must provide a trigger function,
    // which will be called once per period.
    virtual void trigger(int period) = 0;

protected:
    
    bool transferTo(Account *recipient, int amount);
    int balance;
};


// The Firm class is used in Worker, which it also uses, so we
// need a forward declaration here.

class Firm;


// A Worker instance represents an economically active individual.
// 'Economically active' means (a) having an employer (which may
// be null) and (b) having a wage level (which may be zero).
// Workers (who are also consumers -- the terms are effectively
// interchangeble( are managed by the singleton Pool instance.

class Worker : public Account
{
    friend class Pool;
    
private:
    
    int wage;
    Firm *employer;
    
protected:
    
    void setEmployer(Firm*);
    void setWage(int);
    
public:
    
    Worker(int wage, Firm *emp);
    
    int getWage();
    Firm *getEmployer();
    bool isEmployed();
    
    void trigger(int period);
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
    
    int std_wage;
    
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
    
    void trigger(int period);
    
    // Overrides base mmethod to give additional functionality
    void credit(int amount);
};


// Government is a singleton class as we are currently assuming
// a closed economy wth a single currency. Foreign trade would
// require firms that were registered to a different Government.
// This should be added later.

class Government: public Account
{
private:
    
    static Government *_instance;
    
    Firm gov;

    // Firms in a capitalist economy are legal entities and
    // must be registered with the government.
    std::vector<Firm*> firms;
    
    // Probably a temporary measure, this is the amount the
    // government spends per period. It may be better to make
    // it a variable (exogenous) parameter of the system or
    // to determine it algorithmically in some way.
    int exp;
    
protected:
    
    Government();
    
    // The method overrides the method in the base (Account)
    // class, which prohibits transfers that would leave a
    // negative balance. This restriction doesn't apply to
    // the government, which creates money precisely by
    // creating transfers that leave a negative balance.
    void transferTo(Account *recipient, int amount);
    
public:
    
    static Government *Instance();
    
    // Sets the level of government expenditure
    void setExpenditure(int exp);
    
    void trigger(int period);

    ~Government();
};


class Bank: public Account
{
    
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
    
    Worker *hire(int wage, Firm *emp);
    void fire(Worker*);
    
};

#endif /* account_hpp */

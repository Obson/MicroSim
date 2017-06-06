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
    void credit(int amount);
    
    // Every derived class must provide a trigger function,
    // which will be called once per period.
    //
    // TO DO
    // For some reason this causes linker errors in derived classes.
    // As I only included it for the sake of tidiness and it's not
    // strictly necessary I'm commenting it out for the time being.
    // Sort out (or remove) later.
    //
    // virtual void trigger(int period) = 0;

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
    
public:
    
    Firm();
    
    void trigger(int period);
};

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

class Trader: public Account
{
    
};

class Bank: public Account
{
    
};

// Singleton factory class containing all workers
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

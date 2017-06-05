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

class Account
{
public:

    Account();
    int getBalance();
    void credit(int amount);
    
protected:
    
    bool transferTo(Account *recipient, int amount);
    int balance;
};

class Firm; // forward reference

class Worker : public Account
{
private:
    
    int wage;
    Firm *employer;
    
public:
    
    Worker(int wage, Firm *emp);
    
    int getWage();
    Firm *getEmployer();
    bool isEmployed();
};

class Firm: public Account
{
private:
    
    std::vector<Worker *> employees;
};

class Government: public Account
{
private:
    
    static Government *_instance;
    int exp;
    
protected:
    
    Government();
    void transferTo(Account *recipient, int amount);
    
public:
    
    static Government *Instance();
    
    void setExpenditure(int exp);
    void trigger(int period);
    
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
    
    std::vector<Worker*> workers;
    
    static Pool *_instance;

    static int _count;
    
protected:
    
    Pool();
    
public:
    
    static Pool *Instance();
    static void setCount(int count);
    
    ~Pool();
    
    Worker *hire(int wage, Firm *emp);
    
};

#endif /* account_hpp */

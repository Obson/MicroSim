//
//  pool.cpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "account.hpp"

Pool *Pool::_instance = nullptr;
int Pool::_count = -1;

Pool *Pool::Instance()
{
    if (_instance == nullptr)
    {
        _instance = new Pool();
    }
    return _instance;
}

Pool::Pool()
{

}

void Pool::setCount(int count)
{
    if (_count == -1) {
        _count = count;
    }
}

Pool::~Pool()
{
    while (!available.empty())
    {
        delete available.back();
        available.pop_back();
    }
}

Worker *Pool::hire(int wage, Firm *emp)
{
    // At present we don't check the number of workers so we
    // effectively allow an infinite number. Ideally we should check
    // how close to count we are and possibly recruit an existing
    // worker at a higher wage. The point at which we start doing
    // this will be one of the main determinants of inflation.
    
    Worker *w;
    
    // Workers in the available list have already established wage
    // levels, so they may or may not match the current request.
    // However, for the time being we will assume they are in fact
    // interchangeable and simply adjust their wage levels to match
    // the offer. This needs to be made much more sophisticated in
    // order to reflect probable real-world behabiour.
    
    if (!available.empty())
    {
        w = available.back();
        w->setWage(wage);
        w->setEmployer(emp);
        available.pop_back();
    }
    else
    {
        w = new Worker(wage, emp);
    }
    
    return w;
}

void Pool::fire(Worker *w)
{
    w->setEmployer(NULL);
    available.push_back(w);
}

//
//  pool.cpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "account.hpp"

Pool *Pool::_instance = 0;
int Pool::_count = -1;

Pool *Pool::Instance()
{
    if (_instance == 0)
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
    // Free memory. Because we delete all available here there is no
    // need for it to be done elsewhere
    
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
    
    // IMPORTANT NOTE
    //
    // Workers in the available list have already established wage
    // levels, so we cannot just assume they will be suitable as
    // employees for the current request. However, for the time
    // being we will assume they are in fact interchangeable and
    // simply adjust their wage level to match the offer. This
    // needs to be made much more sophisticated to reflact
    // probable real-world behabiour.
    
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

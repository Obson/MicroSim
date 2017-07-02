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
    if (_instance == nullptr) {
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
    // Free up memory
    while (!available.empty()) {
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

void Pool::fire(Worker *w)
{
    w->setEmployer(nullptr);
    available.push_back(w);
}

void Pool::trigger(int period)
{
    for (auto it : available) {
        it->trigger(period);
    }
}

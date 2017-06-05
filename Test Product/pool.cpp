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
    // Free memory. Because we delete all workers here there is no
    // need for it to be done elsewhere
    
    while (!workers.empty())
    {
        delete workers.back();
        workers.pop_back();
    }
}

Worker *Pool::hire(int wage, Firm *emp)
{
    // At present we don't check the number of workers so we
    // effectively allow an infinite number. Ideally we should check
    // how close to count we are and possibly recruit an existing
    // worker at a higher wage. The point at which we start doing
    // this will be one of the main determinants of inflation.
    
    Worker *w = new Worker(wage, emp);
    workers.push_back(w);
    return w;
}

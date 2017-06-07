//
//  worker.cpp
//  Test Product
//
//  Created by David Brown on 05/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "account.hpp"

Worker::Worker(int wage, Firm *emp)
{
    this->wage = wage;
    employer = emp;
}

int Worker::getWage()
{
    return wage;
}

bool Worker::isEmployed()
{
    return (employer != NULL);
}


Firm *Worker::getEmployer()
{
    return employer;
}

void Worker::trigger(int period)
{
    std::cout << "Worker: triggered with balance " << balance << "\n";
    
    // For initial testing (only) we will assume all workers spend all
    // their income. For the sake of simplicity we will also assume that
    // in any give period any given worker spends his/her funds through
    // a single randomly selected firm. As long as there are a lot of
    // workers this should be equivalent to each worker choosing a
    // random selection of firms.
    
    Government *government = Government::Instance();
    transferTo(government->getRandomFirm(), balance);
}

void Worker::setEmployer(Firm *emp)
{
    employer = emp;
}

void Worker::setWage(int wage)
{
    this->wage = wage;
}

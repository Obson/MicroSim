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

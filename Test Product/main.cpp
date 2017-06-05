//
//  main.cpp
//  Test Product
//
//  Created by David Brown on 03/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include <iostream>
#include "account.hpp"

int main(int argc, const char * argv[]) {

    int iters, count, exp;
    
    // Announcement
    std::cout << "MicroSim\n";
    std::cout << "Number of iterations? ";
    std::cin >> iters;
    std::cout << "Number of workers/consumers? ";
    std::cin >> count;
    std::cout << "Government expenditure per period: ";
    std::cin >> exp;
    std::cout << "\n";
    
    Pool::setCount(count);
    
    // Seed the random number generator
    std::srand(42);
    
    Pool *pool = Pool::Instance();
    
    Government *gov = Government::Instance();
    gov->setExpenditure(exp);
    
    for (int period = 1; period <= iters; period++)
    {
        std::cout << period << ". ";
        gov->trigger(period);
    }

    delete pool;
    return 0;
}

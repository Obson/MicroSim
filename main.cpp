//
//  main.cpp
//  Test Product
//
//  Created by David Brown on 03/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include <iostream>
#include "register.hpp"
#include "settings.hpp"

//#include "account.hpp"

int main(int argc, const char * argv[]) {

    // Seed the random number generator
    std::srand(42);
    
    int iters;
    
    std::cout << "MicroSim Version 0.1\n\n";
    std::cout << "Number of weeks: ";

    std::cin >> iters;

    // Settings must be created before any accounts are created so that
    // it can input the settings before they are needed.
    Settings *settings = Settings::Instance();
    
    Statistics *stats = Statistics::Instance();
    
    Register *reg = Register::Instance();

    // Create the Government. This will automatically set up a single firm
    // representing nationalised industries, civil service, and any other
    // government owned business. Note that the Government itself is not
    // a business and taxation is not a payment for goods or services.
    Government *gov = Government::Instance();

    for (int period = 1; period <= iters; period++)
    {
        gov->trigger(period);
        reg->trigger(period);
        stats->next(period);
        if (std::rand() % 100 < settings->getFCP()) {
            reg->createFirm();
        }
    }
    
    stats->report();

    delete gov;
    return 0;
}

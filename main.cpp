//
//  main.cpp
//  Test Product
//
//  Created by David Brown on 03/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <iomanip>
#include <iostream>
#include <sstream>

#include "register.hpp"
#include "settings.hpp"

//#include "account.hpp"

int main(int argc, const char * argv[]) {

    if (argc != 3) {
        std::cout   << "MicroSim version 0.1"
                    << "\nUsage:"
                    << "\n\tmicrosim <iterations> <input-file>\n";
        exit(1);
    }
    // Seed the random number generator
    std::srand(42);
    
    int iters = atoi(argv[1]);
    
    if (iters < 2) {
        std::cout << "microsim: minimum 3 iterations (" << iters << " requested)\n";
    }
    
    // TODO: Add a -v option eventually
    // std::cout << "MicroSim Version 0.1\n\n";
    
    // TODO: Add a -d option for this
    // char the_path[256];
    // getcwd(the_path, 255);
    // std::cout << "\nWorking directory is " << the_path << "\n";
    
    // Settings must be created before any accounts are created so that
    // it can input the settings before they are needed.
    Settings *settings = Settings::Instance(argv[2]);
    
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

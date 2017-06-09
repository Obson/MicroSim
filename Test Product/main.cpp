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

    int iters, count, exp, std_wage, prop_con, inc_tax_rate, sales_tax_rate;
    
    Settings *settings = Settings::Instance();
    
    // Announcement
    std::cout << "MicroSim\n";
    std::cout << "Number of iterations: ";
    std::cin >> iters;
    std::cout << "Size of population: ";
    std::cin >> count;
    std::cout << "Government expenditure per period: ";
    std::cin >> exp;
    std::cout << "Standard wage (after tax): ";
    std::cin >> std_wage;
    std::cout << "Propensity to consume (percent): ";
    std::cin >> prop_con;
    std::cout << "Income tax rate (percent): ";
    std::cin >> inc_tax_rate;
    std::cout << "Sales tax rate (percent): ";
    std::cin >> sales_tax_rate;
    
    // Set global parameters
    settings->prop_con = prop_con;
    settings->inc_tax = inc_tax_rate;
    settings->sales_tax = sales_tax_rate;
    
    // Seed the random number generator
    std::srand(42);
    
    Pool::setCount(count);
    Pool *pool = Pool::Instance();

    Government::setExpenditure(exp);
    Government::setStandardWage(std_wage);
    
    Government *gov = Government::Instance();
    
    /**
     Important!
     ----------
     Do we need to keep a separate list of accounts so they can be triggered
     independently of their owners? This would cause some complications as
     we need to ensure that they are triggered in the order: Government, 
     Firms, Workers, Pool.available, so maybe Government and Pool are the
     only ones that need to be triggered externally and the others could be
     cascaded.
    **/
    
    for (int period = 1; period <= iters; period++)
    {
        std::cout << "\nPeriod " << period << "\n";
        gov->trigger(period);
    }

    std::cout << "Done -- clearing memory\n\n";
    delete pool;
    return 0;
}

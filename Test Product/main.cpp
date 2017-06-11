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

    int iters, count, exp, std_wage, prop_con, dedns, inc_tax_rate, sales_tax_rate;
    
    Settings *settings = Settings::Instance();
    Statistics *stats = Statistics::Instance();
    
    // Announcement
    std::cout << "MicroSim\n\n";
    std::cout << "Number of iterations: ";
    std::cin >> iters;
    std::cout << "Size of population: ";
    std::cin >> count;
    std::cout << "Government expenditure per period: ";
    std::cin >> exp;
    std::cout << "Standard wage (before tax): ";
    std::cin >> std_wage;
    prop_con = 0;
    while (prop_con < 1 || prop_con > 100) {
        std::cout << "Propensity to consume (percent): ";
        std::cin >> prop_con;
    }
    dedns = -1;
    while (dedns < 0 || dedns > 100) {
        std::cout << "Deductions (percent): ";
        std::cin >> dedns;
    }
    inc_tax_rate = -1;
    while (inc_tax_rate < 0 || inc_tax_rate > 100) {
        std::cout << "Income tax rate (percent): ";
        std::cin >> inc_tax_rate;
    }
    sales_tax_rate = -1;
    while (sales_tax_rate < 0 || sales_tax_rate > 100) {
        std::cout << "Sales tax rate (percent): ";
        std::cin >> sales_tax_rate;
    }
    
    // Set global parameters
    settings->prop_con = prop_con;
    settings->inc_tax = inc_tax_rate;
    settings->sales_tax = sales_tax_rate;
    settings->dedns = dedns;
    
    // Seed the random number generator
    std::srand(42);
    
    Pool::setCount(count);
    Pool *pool = Pool::Instance();

    Government::setExpenditure(exp);
    Government::setStandardWage(std_wage);
    
    Government *gov = Government::Instance();
    

    // We don't need to keep a separate list of Accounts as they can be
    // triggered by their owners. We need to ensure that they are triggered
    // in the order: Government, Firms, Workers, Pool.available, so
    // Government and Pool are the only ones that need to be triggered
    // externally. The others can be cascaded.
    
    for (int period = 1; period <= iters; period++)
    {
        std::cout << "\nPeriod " << period << "\n";
        gov->trigger(period);
        pool->trigger(period);
        stats->next(period);
    }
    
    stats->report();

    std::cout << "\nDone -- clearing memory\n\n";
    delete pool;
    return 0;
}

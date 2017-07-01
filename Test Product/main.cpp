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

    int iters, count, exp, std_wage, prop_con, dedns, inc_tax_rate, sales_tax_rate, firm_creation_prob;
    
    // Create static objects
    Settings *settings = Settings::Instance();
    Statistics *stats = Statistics::Instance();
    
    std::cout << "MicroSim Version 0.1\n\n";

    // Get parameters from console input. Eventually this will probably
    // be replaced by a config file.
    std::cout << "Number of weeks: ";
    std::cin >> iters;
    std::cout << "Size of population: ";
    std::cin >> count;
    std::cout << "Government expenditure per week: ";
    std::cin >> exp;
    std::cout << "Standard weekly wage (before tax): ";
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
    firm_creation_prob = -1;
    while (firm_creation_prob < 0 || firm_creation_prob > 100) {
        std::cout << "Firm creation probability (percent): ";
        std::cin >> firm_creation_prob;
    }
    
    // Add parameters to Settings so they are available globally.
    settings->prop_con = prop_con;
    settings->inc_tax = inc_tax_rate;
    settings->sales_tax = sales_tax_rate;
    settings->dedns = dedns;
    
    // Seed the random number generator
    std::srand(42);
    
    // Create an empty Pool
    Pool::setCount(count);
    Pool *pool = Pool::Instance();

    // Create the Government. This will automatically set up a single firm
    // representing nationalised industries, civil service, and any other
    // government owned business. Note that the Government itself is not
    // a business and taxation is not a payment for goods or services.
    Government::setExpenditure(exp);
    Government::setStandardWage(std_wage);
    Government *gov = Government::Instance();

    for (int period = 1; period <= iters; period++)
    {
        gov->trigger(period);
        pool->trigger(period);
        stats->next(period);
        if (std::rand() % 100 < firm_creation_prob) {
            gov->createFirm();
        }
    }
    
    stats->report();

    delete pool;
    return 0;
}

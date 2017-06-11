//
//  statistics.cpp
//  Test Product
//
//  Created by David Brown on 10/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "statistics.hpp"

Statistics *Statistics::_instance = nullptr;

Statistics *Statistics::Instance()
{
    if (_instance == nullptr) {
        _instance = new Statistics();
    }
    return _instance;
}

Statistics::Statistics()
{
    current = new fields;
    *current = {};
}

void Statistics::next(int period)
{
    stats.push_back(current);
    current = new fields;
    *current = {};
}

void Statistics::report()
{
    std::cout << "\nStatistics\n";

    int period = 0;
    for (auto it : stats) {
        std::cout   << "\nPeriod " << ++period << "\n---------\n";
        std::cout   << "\nGovernment\n"
                    << "\tExpenditure = " << it->gov_exp
                    << "\tTax/dedns recd = " << it->tax_recd
                    << "\tDeficit = " << (it->gov_exp - it->tax_recd)
                    << "\tSector balance = " << it->gov_bal
                    << "\n";
        std::cout   << "\nFirms\n"
                    << "\tNumber of employees = " << it->num_employed
                    << "\tNumber hired = " << it->num_hired
                    << "\tNumber fired = " << it->num_fired
                    << "\tWages (after dedns) = " << it->wages_paid
                    << "\tDeductions = " << it->dedns_paid
                    << "\tSales = " << it->tot_sales
                    << "\tSector balance = " << it->prod_bal
                    << "\n";
        std::cout   << "\nEmployed Workers\n"
                    << "\tWages recd = " << it->wages_recd
                    << "\tIncome tax paid = " << it->inc_tax_paid
                    << "\tPurchases = " << it->tot_purchases
                    << "\tSector balance = " << it->house_bal
                    << "\n";
        std::cout   << "\nUnemployed Workers\n"
                    << "\n";
    }
}

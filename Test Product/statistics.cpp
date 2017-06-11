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
                    << "\n\tExpenditure = " << it->gov_exp
                    << "\n\tTax/dedns recd = " << it->tax_recd
                    << "\n\tDeficit = " << (it->gov_exp - it->tax_recd)
                    << "\n\tSector balance = " << it->gov_bal
                    << "\n";
        std::cout   << "\nFirms\n"
                    << "\n\tNumber of employees = " << it->num_employed
                    << "\n\tNumber hired = " << it->num_hired
                    << "\n\tNumber fired = " << it->num_fired
                    << "\n\tWages (after dedns) = " << it->wages_paid
                    << "\n\tDeductions = " << it->dedns_paid
                    << "\n\tSales = " << it->tot_sales
                    << "\n\tSector balance = " << it->prod_bal
                    << "\n";
        std::cout   << "\nEmployed Workers\n"
                    << "\n\tStarting balance = " << it->start_bal
                    << "\n\tWages recd = " << it->wages_recd
                    << "\n\tIncome tax paid = " << it->inc_tax_paid
                    << "\n\tPurchases = " << it->tot_purchases
                    << "\n\tSector balance = " << it->house_bal
                    << "\n";
        std::cout   << "\nUnemployed Workers\n"
                    << "\n";
        
        int recon = it->gov_bal + it->prod_bal + it->house_bal;
        
        std::cout   << "Reconciliation\n--------------\n"
                    << it->gov_bal << " + "
                    << it->prod_bal << " + "
                    << it->house_bal << " = "
                    << recon
                    << "\n";
        
        if (recon == 0) {
            std::cout << "Reconciles OK\n";
        } else {
            std::cout << "*** Reconciliation fails ***\n";
        }
    }
}

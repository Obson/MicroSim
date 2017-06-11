//
//  statistics.cpp
//  Test Product
//
//  Created by David Brown on 10/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include <iomanip>
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
                    << "\n\tNumber fired = " << it->num_fired << "\n"
                    << "\n\t" << std::setw(20) << "Starting balance: " << std::setw(9) << it->f_start_bal
                    << "\n\t" << std::setw(20) << "Wages paid: " << std::setw(9) << it->wages_paid
                    << "\n\t" << std::setw(20) << "Dedns paid: " << std::setw(9) << it->dedns_paid
                    << "\n\t" << std::setw(20) << "Sales: " << std::setw(9) << it->tot_sales
                    << "\n\t" << std::setw(20) << "Sales tax paid: " << std::setw(9) << it->sales_tax_paid
                    << "\n\t" << std::setw(20) << "Closing balance: " << std::setw(9) << it->prod_bal
                    << "\n";
        std::cout   << "\nEmployed Workers\n"
                    << "\n\t" << std::setw(20) << "Starting balance: " << std::setw(9) << it->start_bal
                    << "\n\t" << std::setw(20) << "Wages recd: " << std::setw(9) << it->wages_recd
                    << "\n\t" << std::setw(20) << "Income tax paid: " << std::setw(9) << it->inc_tax_paid
                    << "\n\t" << std::setw(20) << "Purchases: " << std::setw(9)  << it->tot_purchases
                    << "\n\t" << std::setw(20) << "Closing balance: " << std::setw(9)  << it->house_bal
                    << "\n\t" << std::setw(20) << " " << std::setw(9)  << "---------"
                    << "\n\t" << std::setw(20) << " " << std::setw(9)  << (it->start_bal + it->wages_recd - it->inc_tax_paid - it->tot_purchases - it->house_bal)
                    << "\n";
        std::cout   << "\nUnemployed Workers\n"
                    << "\n";
        
        int recon = it->gov_bal + it->prod_bal + it->house_bal;
        
        std::cout   << "Sectors\n"
                    << "\n\tG: " << std::setw(9) << it->gov_bal
                    << "\n\tP: " << std::setw(9) << it->prod_bal
                    << "\n\tH: " << std::setw(9) << it->house_bal
                    << "\n\t   ========="
                    << "\n\t   " << std::setw(9) << recon
                    << "\n\t   ---------"
                    << "\n";
        
        if (recon != 0) {
            std::cout << "*** Reconciliation fails ***\n";
        }
    }
}

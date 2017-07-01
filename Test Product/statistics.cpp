//
//  statistics.cpp
//  Test Product
//
//  Created by David Brown on 10/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include <iomanip>
#include <iostream>
#include <fstream>
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
    Government *gov = Government::Instance();
    
    // Close current period stats
    current->prod_bal = gov->getProdBalance();  // closing balance for firms
    int old_prod_bal = current->prod_bal;
    stats.push_back(current);

    // Open next period stats
    current = new fields;
    *current = {};                              // *** may not be necessary ***
    current->f_start_bal = old_prod_bal;        // opening balance for firms (next period)
    current->num_firms = gov->getNumFirms();
    current->num_employed = gov->getNumEmployees();
}

#include <stdio.h>
#include <unistd.h>
#include <string.h>

void Statistics::report()
{
    // Kludge to get the directory containing the output file
    char the_path[256];
    getcwd(the_path, 255);
    
    std::ofstream myfile;
    std::string fname("microsim.csv");
    myfile.open(fname);
    
    if (myfile.is_open()) {
        std::cout << "\nOutput file is " << the_path << "/" << fname << "\n";
        myfile << "\"Period\",\"Gov Bal\",\"Prod Bal\",\"Dom Bal\",\"Gov Exp\",\"Num Firms\",\"Num Empls\",\"Inc Tax\",\"Sales Tax\",\"Dedns\",\"Deficit\",\"Wages\",\"Consumption\"\n";
    } else {
        std::cout << "Cannot open output file\n";
    }

    std::cout << "\nSTATISTICS LOG\n";

    int period = 0;
    for (auto it : stats) {

        if (myfile.is_open()) {
            myfile  << period
            << "," << it->gov_bal
            << "," << it->prod_bal
            << "," << it->start_bal + it->w_start_bal_unemp
            << "," << it->gov_exp
            
            << "," << it->num_firms
            << "," << it->num_employed
            
            << "," << it->inc_tax_paid + it->inc_tax_paid_unemp
            << "," << it->sales_tax_paid + it->sales_tax_paid_unemp
            << "," << it->dedns_paid
            << "," << it->gov_exp - it->tax_recd
            << "," << it->wages_recd
            << "," << it->tot_sales
            << "\n";
        }

        std::cout   << "\nPeriod " << ++period << "\n---------\n";
        
        std::cout   << "\nGovernment\n"
                    << "\n\t" << std::setw(20) << "Expenditure: " << std::setw(9) << it->gov_exp
                    << "\n\t" << std::setw(20) << "Tax/dedns recd: " << std::setw(9) << it->tax_recd << " -"
                    << "\n\t" << std::setw(20) << " " << std::setw(9)  << "---------"
                    << "\n\t" << std::setw(20) << "Deficit: " << std::setw(9) << (it->gov_exp - it->tax_recd) << "\n"
                    << "\n\t" << std::setw(20) << "Sector balance: " << std::setw(9) << it->gov_bal
                    << "\n";
        std::cout   << "\nFirms\n"
                    << "\n\t" << std::setw(20) << "Number of firms: " << std::setw(9) << it->num_firms
                    << "\n\t" << std::setw(20) << "Existing employees: " << std::setw(9) << it->num_employed
                    << "\n\t" << std::setw(20) << "Hired this week: " << std::setw(9) << it->num_hired
                    << "\n\t" << std::setw(20) << "Fired this week: " << std::setw(9) << it->num_fired << "\n"
        
                    << "\n\t" << std::setw(20) << "Starting balance: " << std::setw(9) << it->f_start_bal
                    << "\n\t" << std::setw(20) << "Government grant: " << std::setw(9) << it->gov_grant << " +"
                    << "\n\t" << std::setw(20) << "Wages paid: " << std::setw(9) << it->wages_paid << " -"
                    << "\n\t" << std::setw(20) << "Dedns paid: " << std::setw(9) << it->dedns_paid << " -"
                    << "\n\t" << std::setw(20) << "Sales: " << std::setw(9) << it->tot_sales << " +"
                    << "\n\t" << std::setw(20) << "Sales tax paid: " << std::setw(9) << it->sales_tax_paid << " -"
                    << "\n\t" << std::setw(20) << "Closing balance: " << std::setw(9) << it->prod_bal << " -"
                    << "\n\t" << std::setw(20) << " " << std::setw(9)  << "---------"
                    << "\n\t" << std::setw(20) << " " << std::setw(9)
                        << (it->f_start_bal + it->gov_grant - it->wages_paid - it->dedns_paid + it->tot_sales - it->sales_tax_paid - it->prod_bal)
                        << "\n";
        std::cout   << "\nEmployed Workers\n"
                    << "\n\t" << std::setw(20) << "Starting balance: " << std::setw(9) << it->start_bal
                    << "\n\t" << std::setw(20) << "Wages recd: " << std::setw(9) << it->wages_recd << " +"
                    << "\n\t" << std::setw(20) << "Income tax paid: " << std::setw(9) << it->inc_tax_paid << " -"
                    << "\n\t" << std::setw(20) << "Purchases: " << std::setw(9)  << it->tot_purchases << " -"
                    << "\n\t" << std::setw(20) << "Closing balance: " << std::setw(9)  << it->house_bal << " -"
                    << "\n\t" << std::setw(20) << " " << std::setw(9)  << "---------"
                    << "\n\t" << std::setw(20) << " " << std::setw(9)  << (it->start_bal + it->wages_recd - it->inc_tax_paid - it->tot_purchases - it->house_bal)
                    << "\n";
        std::cout   << "\nUnemployed Workers\n"
                    << "\n\t" << std::setw(20) << "Starting balance: " << std::setw(9) << it->w_start_bal_unemp
                    << "\n\t" << std::setw(20) << "Purchases: " << std::setw(9)  << it->tot_purch_unemp
                    << "\n\t" << std::setw(20) << "Ending balance: " << std::setw(9) << it->w_end_bal_unemp
                    << "\n";
        
        int recon = it->gov_bal + it->prod_bal + it->house_bal + it->w_end_bal_unemp;
        
        std::cout   << "\nSectors\n"
                    << "\n\t" << std::setw(20) << "Government: " << std::setw(9) << it->gov_bal
                    << "\n\t" << std::setw(20) << "Producers: " << std::setw(9) << it->prod_bal
                    << "\n\t" << std::setw(20) << "Employed: " << std::setw(9) << it->house_bal
                    << "\n\t" << std::setw(20) << "Unemployed: " << std::setw(9) << it->w_end_bal_unemp
                    << "\n\t" << std::setw(20) << "  " << std::setw(9) << "---------"
                    << "\n\t" << std::setw(20) << "  " << std::setw(9) << recon
                    << "\n\t" << std::setw(20) << "  " << std::setw(9) << "---------"
                    << "\n";
        
        if (recon != 0) {
            std::cout << "*** Reconciliation fails ***\n";
        }
        
    }

    if (myfile.is_open()) {
        myfile.close();
    }
}

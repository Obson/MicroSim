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
    current = new Fields;
    //*current = {};
}

// This is called BEFORE the next period is triggered. It dumps current stats
// and creates a new record for the next period, entering BFwd amounts from
// the CFwd amounts from the previous period where appropriate.
void Statistics::next(int period)
{
    Government *gov = Government::Instance();

    stats.push_back(current);
    Fields *previous = current;
    current = new Fields;

    // Gov
    previous->gov.cfwd = gov->getBalance();
    previous->gov.exp = gov->getExpenditure();
    previous->gov.rec = gov->getReceipts();
    previous->gov.num_firms = gov->getNumFirms();
    previous->gov.num_emps = gov->getNumEmployees();
    previous->gov.hires = gov->getNumHired();
    previous->gov.fires = gov->getNumFired();
    //
    current->gov.bfwd = previous->gov.cfwd;
    
    // Prod
    previous->prod.cfwd = gov->getProdBalance();
    previous->prod.grant = gov->getGrantsRecd();
    previous->prod.dedns = gov->getDednsPaid();
    previous->prod.wages = gov->getWagesPaid();
    previous->prod.sales = gov->getPurchases();
    previous->prod.sales_tax = gov->getSalesTaxPaid();
    //
    current->prod.bfwd = previous->prod.cfwd;
    
    // Emp
    previous->emp.close = gov->getEmpBal();
    previous->emp.wages = gov->getWagesPaid();
    previous->emp.inc_tax = gov->getIncTaxPaid();
    previous->emp.purch = gov->getEmpPurch();
    // *
    current->emp.start = previous->emp.close;
    
    // Unemp
    previous->unemp.close = gov->getUnempBal();
    previous->unemp.benefits = gov->getBenefitsRecd();
    previous->unemp.purchases = gov->getUnempPurch();
    // *
    current->unemp.start = previous->unemp.close;
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
        myfile << "\"Period\",\"Gov Bal\",\"Prod Bal\",\"Dom Bal\",\"Gov Exp\",\"Num Firms\",\"Num Empls\",\"Inc Tax\",\"Sales Tax\",\"Dedns\",\"Deficit\",\"Wages\",\"Benefits\",\"Consumption\"\n";
    } else {
        std::cout << "Cannot open output file\n";
    }

    std::cout << "\nSTATISTICS LOG\n";

    int period = 0;
    for (auto it : stats) {

        if (myfile.is_open()) {
            myfile  << period
                    << "," << it->gov.cfwd
                    << "," << it->prod.cfwd
                    << "," << it->emp.close + it->unemp.close
                    << "," << it->gov.exp
            
                    << "," << it->gov.num_firms
                    << "," << it->gov.num_emps
            
                    << "," << it->emp.inc_tax
                    << "," << it->prod.sales_tax
                    << "," << it->prod.dedns
                    << "," << it->gov.exp - it->gov.rec
                    << "," << it->emp.wages
                    << "," << it->unemp.benefits
                    << "," << it->emp.purch + it->unemp.purchases
                    << "\n";
        }

        std::cout   << "\nPeriod " << ++period << "\n---------\n";
        
        std::cout   << "\nGovernment\n"
                    << "\n\t" << std::setw(21) << "Expenditure: " << std::setw(9) << it->gov.exp
                    << "\n\t" << std::setw(21) << "Receipts: " << std::setw(9) << it->gov.rec << " -"
                    << "\n\t" << std::setw(21) << " " << std::setw(9)  << "---------"
                    << "\n\t" << std::setw(21) << "Deficit: " << std::setw(9) << (it->gov.exp - it->gov.rec) << "\n"
                    << "\n\t" << std::setw(21) << "Sector balance: " << std::setw(9) << it->gov.cfwd
                    << "\n";
        std::cout   << "\nFirms\n"
                    << "\n\t" << std::setw(21) << "Number of firms: " << std::setw(9) << it->gov.num_firms
                    << "\n\t" << std::setw(21) << "Number of employees: " << std::setw(9) << it->gov.num_emps
                    << "\n\t" << std::setw(21) << "Hired this week: " << std::setw(9) << it->gov.hires
                    << "\n\t" << std::setw(21) << "Fired this week: " << std::setw(9) << it->gov.fires << "\n"
        
                    << "\n\t" << std::setw(21) << "Starting balance: " << std::setw(9) << it->prod.bfwd
                    << "\n\t" << std::setw(21) << "Government grant: " << std::setw(9) << it->prod.grant << " +"
                    << "\n\t" << std::setw(21) << "Wages paid: " << std::setw(9) << it->prod.wages << " -"
                    << "\n\t" << std::setw(21) << "Dedns paid: " << std::setw(9) << it->prod.dedns << " -"
                    << "\n\t" << std::setw(21) << "Sales: " << std::setw(9) << it->prod.sales << " +"
                    << "\n\t" << std::setw(21) << "Sales tax paid: " << std::setw(9) << it->prod.sales_tax << " -"
                    << "\n\t" << std::setw(21) << "Closing balance: " << std::setw(9) << it->prod.cfwd << " -"
                    << "\n\t" << std::setw(21) << " " << std::setw(9)  << "---------"
                    << "\n\t" << std::setw(21) << " " << std::setw(9)
                        << (it->prod.bfwd + it->prod.grant - it->prod.wages - it->prod.dedns + it->prod.sales - it->prod.sales_tax - it->prod.cfwd)
                        << "\n";
        std::cout   << "\nEmployed Workers\n"
                    << "\n\t" << std::setw(21) << "Starting balance: " << std::setw(9) << it->emp.start
                    << "\n\t" << std::setw(21) << "Wages recd: " << std::setw(9) << it->emp.wages << " +"
                    << "\n\t" << std::setw(21) << "Income tax paid: " << std::setw(9) << it->emp.inc_tax << " -"
                    << "\n\t" << std::setw(21) << "Purchases: " << std::setw(9)  << it->emp.purch << " -"
                    << "\n\t" << std::setw(21) << "Closing balance: " << std::setw(9)  << it->emp.close << " -"
                    << "\n\t" << std::setw(21) << " " << std::setw(9)  << "---------"
                    << "\n\t" << std::setw(21) << " " << std::setw(9)  << (it->emp.start + it->emp.wages - it->emp.inc_tax - it->emp.purch - it->emp.close)
                    << "\n";
        std::cout   << "\nUnemployed Workers\n"
                    << "\n\t" << std::setw(21) << "Starting balance: " << std::setw(9) << it->unemp.start
                    << "\n\t" << std::setw(21) << "Benefits: " << std::setw(9)  << it->unemp.benefits << " +"
                    << "\n\t" << std::setw(21) << "Purchases: " << std::setw(9)  << it->unemp.purchases << " -"
                    << "\n\t" << std::setw(21) << "Ending balance: " << std::setw(9) << it->unemp.close << " -"
                    << "\n\t" << std::setw(21) << " " << std::setw(9)  << "---------"
                    << "\n\t" << std::setw(21) << " " << std::setw(9)  << (it->unemp.start + it->unemp.benefits - it->unemp.purchases - it->unemp.close)
                    << "\n\t" << std::setw(21) << " " << std::setw(9)  << "---------"
                    << "\n";
        
        int recon = it->gov.cfwd + it->prod.cfwd + it->emp.close + it->unemp.close;
        
        std::cout   << "\nSectors\n"
                    << "\n\t" << std::setw(21) << "Government: " << std::setw(9) << it->gov.cfwd
                    << "\n\t" << std::setw(21) << "Producers: " << std::setw(9) << it->prod.cfwd
                    << "\n\t" << std::setw(21) << "Employed: " << std::setw(9) << it->emp.close
                    << "\n\t" << std::setw(21) << "Unemployed: " << std::setw(9) << it->unemp.close
                    << "\n\t" << std::setw(21) << "  " << std::setw(9) << "---------"
                    << "\n\t" << std::setw(21) << "  " << std::setw(9) << recon
                    << "\n\t" << std::setw(21) << "  " << std::setw(9) << "---------"
                    << "\n";
        
        if (recon != 0) {
            std::cout << "*** Reconciliation fails ***\n";
        }
        
    }

    if (myfile.is_open()) {
        myfile.close();
    }
}

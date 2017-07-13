//
//  settings.hpp
//  Test Product
//
//  Created by David Brown on 08/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#ifndef settings_hpp
#define settings_hpp

#include <stdio.h>
#include <iostream>
//#include <string>
#include "account.hpp"

class Settings
{
private:
    
    static Settings *_instance;
    static int id;
    
protected:

    //int exp;
    int count;
    int emp_rate;
    int std_wage;
    int prop_con;
    int inc_tax_rate;
    int sales_tax_rate;
    int dedns;
    int firm_creation_prob;
    int unemp_ben_rate;
    //int population;
    int active_pop;
    int reserve;
    int prop_inv;
    
    static size_t parseLine(std::string &input, std::vector<std::string> &output);
    static bool validatePercent(int n, const std::string &descr, int min = 0, int max = 100);
    
    Settings();
    
public:

    // The filename is ignored on all calls after the first
    static Settings *Instance();
    static std::string fname;
    
    int getPopSize();       // population size
    int getActivePop();     // target size of gov-owned businesses
    int getGovExpRate();    // government expenditure (currency units per period)
    int getStdWage();       // standard wage (currency units per employee per period)
    int getPropCon();       // propensity to consume (%)
    int getIncTaxRate();    // income tax rate (%)
    int getSalesTaxRate();  // sales tax rate (%)
    int getPreTaxDedns();   // pre-tax deductions (%)
    int getFCP();           // firm creaton probability (%)
    int getUBR();           // unemployment benefit rate (% of std wage)
    int getPropInv();       // propensity to invest
    int getReserve();       // funds kept in reserve for next period (%)
    
    static int getId();
};

#endif /* settings_hpp */

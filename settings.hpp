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
#include "account.hpp"

class Settings
{
private:
    
    static Settings *_instance;
    
protected:

    int exp;
    int std_wage;
    int prop_con;
    int inc_tax;
    int sales_tax;
    int dedns;
    int firm_creation_prob;
    int unemp_ben_rate;
    int population;
    
public:

    static Settings *Instance();
    
    int getPopSize();       // population size
    int getGovExpRate();    // government expenditure (currency units per period)
    int getStdWage();       // standard wage (currency units per employee per period)
    int getPropCon();       // propensity to consume (%)
    int getIncTaxRate();    // income tax rate (%)
    int getSalesTaxRate();  // sales tax rate (%)
    int getPreTaxDedns();   // pre-tax deductions (%)
    int getFCP();           // firm creaton probability (%)
    int getUBR();           // unemployment benefit rate (% of std wage)
};

#endif /* settings_hpp */
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
    static int id;
    
protected:

    //int exp;
    int std_wage;
    int prop_con;
    int inc_tax;
    int sales_tax;
    int dedns;
    int firm_creation_prob;
    int unemp_ben_rate;
    int population;
    int gov_pop;
    int prop_inv;
    
public:

    static Settings *Instance();
    
    int getPopSize();       // population size
    int getGovPop();        // target size of gov-owned businesses
    int getGovExpRate();    // government expenditure (currency units per period)
    int getStdWage();       // standard wage (currency units per employee per period)
    int getPropCon();       // propensity to consume (%)
    int getIncTaxRate();    // income tax rate (%)
    int getSalesTaxRate();  // sales tax rate (%)
    int getPreTaxDedns();   // pre-tax deductions (%)
    int getFCP();           // firm creaton probability (%)
    int getUBR();           // unemployment benefit rate (% of std wage)
    int getPropInv();       // propensity to invest
    
    static int getId();
};

#endif /* settings_hpp */

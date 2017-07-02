//
//  settings.cpp
//  Test Product
//
//  Created by David Brown on 08/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "settings.hpp"

Settings *Settings::_instance = nullptr;

Settings *Settings::Instance()
{
    if (_instance == nullptr)
    {
        _instance = new Settings();
        
        // Read settings from console (eventually use config file)
        int count, exp, std_wage, prop_con, dedns, inc_tax_rate, sales_tax_rate,
            firm_creation_prob, unemp_ben_rate;
        
        // Get parameters from console input. Eventually this will probably
        // be replaced by a config file.
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
        unemp_ben_rate = -1;
        while (unemp_ben_rate < 0 || unemp_ben_rate > 100) {
            std::cout << "Unemp benefit rate (percent of std wage): ";
            std::cin >> unemp_ben_rate;
        }
        
        _instance->population = count;
        _instance->exp = exp;
        _instance->std_wage = std_wage;
        _instance->prop_con = prop_con;
        _instance->inc_tax = inc_tax_rate;
        _instance->sales_tax = sales_tax_rate;
        _instance->dedns = dedns;
        _instance->firm_creation_prob = firm_creation_prob;
        _instance->unemp_ben_rate = unemp_ben_rate;
    }
    return _instance;
}

/* 'get' methods
 int getGovExpRate();    // government expenditure (currency units per period)
 int getStdWage();       // standard wage (currency units per employee per period)
 int getPropCon();       // propensity to consume (%)
 int getIncTaxRate();    // income tax rate (%)
 int getSalesTaxRate();  // sales tax rate (%)
 int getPreTaxDedns();   // pre-tax deductions (%)
 int getFCP();           // firm creaton probability (%)
 int getUBR();           // unemployment benefit rate (% of std wage)
*/

int Settings::getPopSize()
{
    return population;
}

int Settings::getGovExpRate()
{
    return exp;
}

int Settings::getStdWage()
{
    return std_wage;
}

int Settings::getPropCon()
{
    return prop_con;
}

int Settings::getIncTaxRate()
{
    return inc_tax;
}

int Settings::getSalesTaxRate()
{
    return sales_tax;
}

int Settings::getPreTaxDedns()
{
    return dedns;
}

int Settings::getFCP()
{
    return firm_creation_prob;
}

int Settings:: getUBR()
{
    return unemp_ben_rate;
}

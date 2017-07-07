//
//  settings.cpp
//  Test Product
//
//  Created by David Brown on 08/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "settings.hpp"

Settings *Settings::_instance = nullptr;
int Settings::id = 0;
int Settings::getId() {
    return ++id;
}

Settings *Settings::Instance()
{
    if (_instance == nullptr)
    {
        _instance = new Settings();
        
        // Get parameters from console input. This can be read from a file
        // by redirecting it to stdin, but a more user-friendly system
        // would be a good idea eventually...
        
        int count, gov_pop, std_wage, prop_con, dedns, inc_tax_rate, sales_tax_rate,
            firm_creation_prob, unemp_ben_rate, prop_inv;
        
        std::cout << "Size of population: ";
        std::cin >> count;
        std::cout << "Target number of govt employees: ";
        std::cin >> gov_pop;
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
        prop_inv = -1;
        while (prop_inv < 0 || prop_inv > 100) {
            std::cout << "Propensity to invest (percent): ";
            std::cin >> prop_inv;
        }
        unemp_ben_rate = -1;
        while (unemp_ben_rate < 0 || unemp_ben_rate > 100) {
            std::cout << "Unemp benefit rate (percent of std wage): ";
            std::cin >> unemp_ben_rate;
        }
        
        _instance->population = count;
        _instance->gov_pop = gov_pop;
        _instance->std_wage = std_wage;
        _instance->prop_con = prop_con;
        _instance->inc_tax = inc_tax_rate;
        _instance->sales_tax = sales_tax_rate;
        _instance->dedns = dedns;
        _instance->firm_creation_prob = firm_creation_prob;
        _instance->unemp_ben_rate = unemp_ben_rate;
        _instance->prop_inv = prop_inv;
    }
    return _instance;
}

int Settings::getPopSize()
{
    return population;
}

int Settings::getGovPop()
{
    return gov_pop;
}

int Settings::getGovExpRate()
{
    // We calculate this rather than holding it as a constant as
    // we may want to apply different rules in the future.
    return (gov_pop * std_wage * inc_tax) / 100;
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

int Settings::getPropInv()
{
    return prop_inv;
}

//
//  settings.cpp
//  Test Product
//
//  Created by David Brown on 08/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include <iomanip>
#include <iostream>
#include <fstream>

#include "settings.hpp"

Settings *Settings::_instance = nullptr;
std::string Settings::fname = "basic.mod";
int Settings::id = 0;
int Settings::getId() {
    return ++id;
}

Settings::Settings()
{
    std::ifstream configfile(fname, std::ios::in);
    std::string line;
    if (!configfile.is_open()) {
        std::cout << "Input file (" << fname << ") not found";
    }
    
    // Defaults:
    iters = 10;
    count = 100000;
    emp_rate = 95;
    std_wage = 500;
    prop_con = 75;
    dedns = 10;
    inc_tax_rate = 10;
    sales_tax_rate = 25;
    firm_creation_prob = 0;
    unemp_ben_rate = 50;
    reserve = 0;
    prop_inv = 100;
    
    bool ok = true;
    
    // TODO: Unhandled exceptions if std::stoi fails
    while (getline(configfile, line)) {
        std::vector<std::string> tuple;
        size_t len = parseLine(line, tuple);
        if (len > 0) {
            //std::string stuple(tuple[0]);
            if (len == 2 && tuple[0] == std::string("Iterations")) {
                iters = std::stoi(tuple[1]);
            } else if (len == 3 && tuple[0] == std::string("Population")) {
                if (tuple[1] == "size") {
                    count = std::stoi(tuple[2]);
                }
            } else if (len == 3 && tuple[0] == "Employment") {
                if (tuple[1] == "rate") {
                    emp_rate = std::stoi(tuple[2]);
                    ok = ok && validatePercent(emp_rate, "employment rate");
                }
            } else if (len == 3 && tuple[0] == "Consumption") {
                if (tuple[1] == "rate") {
                    prop_con = std::stoi(tuple[2]);
                    ok = ok && validatePercent(prop_con, "consumption rate");
                }
            } else if (len == 3 && tuple[0] == "Deductions") {
                if (tuple[1] == "rate") {
                    dedns = std::stoi(tuple[2]);
                    ok = ok && validatePercent(dedns, "deductions rate");
                }
            } else if (len == 3 && tuple[0] == "Income") {
                if (tuple[1] == "tax") {
                    if (tuple[2] == "rate") {
                        inc_tax_rate = std::stoi(tuple[3]);
                        ok = ok && validatePercent(inc_tax_rate, "income tax rate");
                    }
                }
            } else if (len == 4 && tuple[0] == "Sales") {
                if (tuple[1] == "tax") {
                    if (tuple[2] == "rate") {
                        sales_tax_rate = std::stoi(tuple[3]);
                        ok = ok && validatePercent(sales_tax_rate, "sales tax rate");
                    }
                }
            } else if (len == 4 && tuple[0] == "Business") {
                if (tuple[1] == "creation") {
                    if (tuple[2] == "rate") {
                        firm_creation_prob = std::stoi(tuple[3]);
                        ok = ok && validatePercent(firm_creation_prob, "business creation rate");
                    }
                }
            } else if (len == 3 && tuple[0] == "Reserve") {
                if (tuple[1] == "rate") {
                    reserve = std::stoi(tuple[2]);
                    ok = ok && validatePercent(reserve, "reserve rate");
                }
            } else if (len == 3 && tuple[0] == "Investment") {
                if (tuple[1] == "rate") {
                    prop_inv = std::stoi(tuple[2]);
                    ok = ok && validatePercent(prop_inv, "investment rate");
                }
            }
        }
    }
    
    if (ok) {
        active_pop = (count * emp_rate) / 100 ;
    } else {
        exit(2);
    }
    
}

Settings *Settings::Instance()
{
    if (_instance == nullptr) {
        _instance = new Settings();
    }
    return _instance;
}


#include <vector>
#include <string>
#include <sstream>

using namespace std;

size_t Settings::parseLine(std::string &input, std::vector<std::string> &output)
{
    std::string buf; // Have a buffer string
    std::stringstream ss(input); // Insert the string into a stream
    
    while (ss >> buf) {
        output.push_back(buf);
    }
    
    return output.size();
}

bool Settings::validatePercent(int n, const std::string &descr, int min, int max)
{
    if (n < min || n > max) {
        std::cout << n << " is not a valid " << descr << std::endl;
        return false;
    } else {
        return true;
    }
}

int Settings::getIters()
{
    return iters;
}

int Settings::getPopSize()
{
    return count;
}

int Settings::getActivePop()
{
    return active_pop;
}

int Settings::getGovExpRate()
{
    // We calculate this rather than holding it as a constant as
    // we may want to apply different rules in the future.
    return (active_pop * std_wage * inc_tax_rate * prop_inv) / 10000;
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
    return inc_tax_rate;
}

int Settings::getSalesTaxRate()
{
    return sales_tax_rate;
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

int Settings::getReserve()
{
    return reserve;
}

int Settings::getPropInv()
{
    return prop_inv;
}

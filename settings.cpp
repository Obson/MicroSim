//
//  settings.cpp
//  Test Product
//
//  Created by David Brown on 08/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

// ++++
#include <iomanip>
#include <iostream>
#include <fstream>
// ++++

#include "settings.hpp"

Settings *Settings::_instance = nullptr;
int Settings::id = 0;
int Settings::getId() {
    return ++id;
}

Settings *Settings::Instance(std::string fname)
{
    if (_instance == nullptr)
    {
        _instance = new Settings();
        
        std::ifstream configfile(fname, std::ios::in);
        std::string line;
        if (!configfile.is_open()) {
            std::cout << "Input file (" << fname << ") not found";
        }
        
        // Defaults:
        _instance->count = 100000;
        _instance->emp_rate = 95;
        _instance->std_wage = 500;
        _instance->prop_con = 75;
        _instance->dedns = 10;
        _instance->inc_tax_rate = 10;
        _instance->sales_tax_rate = 25;
        _instance->firm_creation_prob = 0;
        _instance->unemp_ben_rate = 50;
        _instance->reserve = 0;
        _instance->prop_inv = 100;
        
        bool ok = true;
        
        // TODO: Unhandled exceptions if std::stoi fails
        while (getline(configfile, line)) {
            std::vector<std::string> tuple;
            size_t len = parseLine(line, tuple);
            if (len > 0) {
                //std::string stuple(tuple[0]);
                if (len == 3 && tuple[0] == std::string("Population")) {
                    if (tuple[1] == "size") {
                        _instance->count = std::stoi(tuple[2]);
                    }
                } else if (len == 3 && tuple[0] == "Employment") {
                    if (tuple[1] == "rate") {
                        _instance->emp_rate = std::stoi(tuple[2]);
                        ok = ok && validatePercent(_instance->emp_rate, "employment rate");
                    }
                } else if (len == 3 && tuple[0] == "Consumption") {
                    if (tuple[1] == "rate") {
                        _instance->prop_con = std::stoi(tuple[2]);
                        ok = ok && validatePercent(_instance->prop_con, "consumption rate");
                    }
                } else if (len == 3 && tuple[0] == "Deductions") {
                    if (tuple[1] == "rate") {
                        _instance->dedns = std::stoi(tuple[2]);
                        ok = ok && validatePercent(_instance->dedns, "deductions rate");
                    }
                } else if (len == 3 && tuple[0] == "Income") {
                    if (tuple[1] == "tax") {
                        if (tuple[2] == "rate") {
                            _instance->inc_tax_rate = std::stoi(tuple[3]);
                            ok = ok && validatePercent(_instance->inc_tax_rate, "income tax rate");
                        }
                    }
                } else if (len == 4 && tuple[0] == "Sales") {
                    if (tuple[1] == "tax") {
                        if (tuple[2] == "rate") {
                            _instance->sales_tax_rate = std::stoi(tuple[3]);
                            ok = ok && validatePercent(_instance->sales_tax_rate, "sales tax rate");
                        }
                    }
                } else if (len == 4 && tuple[0] == "Business") {
                    if (tuple[1] == "creation") {
                        if (tuple[2] == "rate") {
                            _instance->firm_creation_prob = std::stoi(tuple[3]);
                            ok = ok && validatePercent(_instance->firm_creation_prob, "business creation rate");
                        }
                    }
                } else if (len == 3 && tuple[0] == "Reserve") {
                    if (tuple[1] == "rate") {
                        _instance->reserve = std::stoi(tuple[2]);
                        ok = ok && validatePercent(_instance->reserve, "reserve rate");
                    }
                } else if (len == 3 && tuple[0] == "Investment") {
                    if (tuple[1] == "rate") {
                        _instance->prop_inv = std::stoi(tuple[2]);
                        ok = ok && validatePercent(_instance->prop_inv, "investment rate");
                    }
                }
            }
        }
        
        if (!ok) {
            exit(2);
        }
        
        _instance->active_pop = _instance->count * _instance->emp_rate;

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

int Settings::getPopSize()
{
    return population;
}

int Settings::getActivePop()
{
    return active_pop;
}

int Settings::getGovExpRate()
{
    // We calculate this rather than holding it as a constant as
    // we may want to apply different rules in the future.
    return (active_pop * std_wage * inc_tax_rate) / 100;
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

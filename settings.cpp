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
int Settings::period = 0;

Settings::Params::Params()
{
    // This constructor is probably unnecessary.
}

Settings::Settings()
{
    std::ifstream configfile(fname, std::ios::in);
    std::string line;
    if (!configfile.is_open()) {
        std::cout << "Input file (" << fname << ") not found";
    }
    
    reg = Register::Instance();
    
    bool ok = true;
    
    Params *params = new Params;
    
    // TODO: Unhandled exceptions if std::stoi fails
    while (getline(configfile, line)) {
        std::vector<std::string> tuple;
        size_t len = parseLine(line, tuple);
        if (len > 0)
        {
            if (len == 4 && tuple[0] == "when")
            {
                // Start new parameter set
                cond.push_back(params);
                params = new Params;
                
                // Set the condition for the parameter set
                Property p = parsePropertyName(tuple[1]);
                params->condition.property = p;
                if (p != dflt) {
                    Opr op = parseOperator(tuple[2]);
                    params->condition.opr = op;
                    if (op != invalid_op) {
                        params->condition.val = std::stoi(tuple[3]);
                    }
                }
                
            } else if (len == 2 && tuple[0] == std::string("Iterations")) {
                params->iters.val = std::stoi(tuple[1]);
                params->iters.is_set = true;
            } else if (len == 3 && tuple[0] == std::string("Population")) {
                if (tuple[1] == "size") {
                    params->count.val = std::stoi(tuple[2]);
                    params->count.is_set = true;
                }
            } else if (len == 3 && tuple[0] == "Employment") {
                if (tuple[1] == "rate") {
                    params->emp_rate.val = std::stoi(tuple[2]);
                    params->emp_rate.is_set = true;
                    ok = ok && validatePercent(params->emp_rate.val, "employment rate");
                }
            } else if (len == 3 && tuple[0] == "Consumption") {
                if (tuple[1] == "rate") {
                    int val = std::stoi(tuple[2]);
                    if (validatePercent(val, "consumption rate")) {
                        params->prop_con.val = val;
                        params->prop_con.is_set = true;
                    } else {
                        ok = false;
                    }
                }
            } else if (len == 3 && tuple[0] == "Deductions") {
                if (tuple[1] == "rate") {
                    int val = std::stoi(tuple[2]);
                    if (validatePercent(val, "deductions rate")) {
                        params->dedns.val = val;
                        params->dedns.is_set = true;
                    } else {
                        ok = false;
                    }
                }
            } else if (len == 4 && tuple[0] == "Income") {
                if (tuple[1] == "tax") {
                    if (tuple[2] == "rate") {
                        int val = std::stoi(tuple[3]);
                        if (validatePercent(val, "income tax rate")) {
                            params->inc_tax_rate.val = val;
                            params->inc_tax_rate.is_set = true;
                        } else {
                            ok = false;
                        }
                    }
                }
            } else if (len == 4 && tuple[0] == "Sales") {
                if (tuple[1] == "tax") {
                    if (tuple[2] == "rate") {
                        int val = std::stoi(tuple[3]);
                        if (validatePercent(val, "sales tax rate")) {
                            params->sales_tax_rate.val = val;
                            params->sales_tax_rate.is_set = true;
                        } else {
                            ok = false;
                        }
                    }
                }
            } else if (len == 4 && tuple[0] == "Business") {
                if (tuple[1] == "creation") {
                    if (tuple[2] == "rate") {
                        int val = std::stoi(tuple[3]);
                        if (validatePercent(val, "business creation rate")) {
                            params->firm_creation_prob.val = val;
                            params->firm_creation_prob.is_set = true;
                        } else {
                            ok = false;
                        }
                    }
                }
            } else if (len == 3 && tuple[0] == "Reserve") {
                if (tuple[1] == "rate") {
                    int val = std::stoi(tuple[2]);
                    if (validatePercent(val, "reserve rate")) {
                        params->reserve.val = val;
                        params->reserve.is_set = true;
                    } else {
                        ok = false;
                    }
                }
            } else if (len == 3 && tuple[0] == "Investment") {
                if (tuple[1] == "rate") {
                    int val = std::stoi(tuple[2]);
                    if (validatePercent(val, "investment rate")) {
                        params->prop_inv.val = val;
                        params->prop_inv.is_set = true;
                    } else {
                        ok = false;
                    }
                }
            } else if (len == 4 && tuple[0] == "Unemployment") {
                if (tuple[1] == "benefit") {
                    if (tuple[2] == "rate") {
                        int val = std::stoi(tuple[3]);
                        if (validatePercent(val, "unemployment benefit rate")) {
                            params->unemp_ben_rate.val = val;
                            params->unemp_ben_rate.is_set = true;
                        } else {
                            ok = false;
                        }
                    }
                }
            }
        }
    }
    
    if (ok) {
        if (params->active_pop.is_set) {
            params->active_pop.val = (params->count.val * params->emp_rate.val) / 100;
            params->active_pop.is_set = true;
        }
        cond.push_back(params);
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

//using namespace std;

Settings::Property Settings::parsePropertyName(std::string s)
{
    if (s == "iterations" || s == "period" | s == "cycles") {
        return current_period;
    } else if (s == "government-expenditure" || s == "gov-exp" || s == "exp") {
        return gov_exp;
    } else if (s == "benefits" || s == "benefits-paid") {
        return bens_paid;
    } else if (s == "government-receipts" || s == "receipts" || s == "rcpts") {
        return gov_recpts;
    } else if (s == "defecit") {
        return deficit;
    } else if (s == "G"|| s == "government-balance" || s == "government-sector") {
        return gov_bal;
    } else if (s == "firms" || s == "businesses") {
        return num_firms;
    } else if (s == "employed") {
        return num_emps;
    } else if (s == "unemployed") {
        return num_unemps;
    } else if (s == "government-employed") {
        return num_gov_emps;
    } else if (s == "hired") {
        return num_hired;
    } else if (s == "fired") {
        return num_fired;
    } else if (s == "P" || s == "business-sector") {
        return prod_bal;
    } else if (s == "wages") {
        return wages;
    } else if (s == "consumption") {
        return consumption;
    } else if (s == "bonuses") {
        return bonuses;
    } else if (s == "deductions" || s == "dedns") {
        return dedns;
    } else if (s == "income-tax") {
        return inc_tax;
    } else if (s == "sales-tax") {
        return sales_tax;
    } else if (s == "H" || s == "domestic-sector") {
        return dom_bal;
    } else {
        return dflt;
    }
}

Settings::Opr Settings::parseOperator(std::string s)
{
    if (s == "=" || s == "is" || s == "equals") {
        return eq;
    } else if (s == "!=" || s == "<>" || s == "not") {
        return neq;
    } else if (s == "<" || s == "under" || s == "below") {
        return lt;
    } else if (s == ">" || s == "exceeds" || s == "over") {
        return gt;
    } else if (s == "<=" || s == "upto" || s == "up-to") {
        return leq;
    } else if (s == ">=" || s == "atleast" || s == "at-least") {
        return geq;
    } else {
        return invalid_op;
    }
}

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

bool Settings::compare(int lhs, int rhs, Opr opr)
{
    switch (opr) {
        case eq:
            return lhs == rhs;
        case neq:
            return lhs != rhs;
        case lt:
            return lhs < rhs;
        case gt:
            return lhs > rhs;
        case leq:
            return lhs <= rhs;
        case geq:
            return lhs >= rhs;
        case invalid_op:
            return false;
    }
}

bool Settings::applies(Settings::Condition &condition)
{
    if (condition.property == dflt) {
        return true;
    } else {
        int lhs = getPropertyVal(condition.property);
        Opr opr = condition.opr;
        int rhs = condition.val;
        return compare(lhs, rhs, opr);
    }
}

int Settings::getIters()
{
    int iters = 10;         // default
    for (auto it : cond) {
        if (applies(it->condition) && it->iters.is_set) {
            iters = it->iters.val;
        }
    }
    return iters;
}

int Settings::getPopSize()
{
    int count = 1000;
    for (auto it : cond) {
        if (applies(it->condition) && it->count.is_set) {
            count = it->count.val;
        }
    }
    return count;
}

int Settings::getActivePop()
{
    return getParameterVal(pt_active_pop, 1000);
}

int Settings::getGovExpRate()
{
    return (getActivePop() * getStdWage() * getIncTaxRate() * getPropInv()) / 10000;
}

// Not to be confused with parameter values, this function returns the actual
// values of the properties of the model when the function is called. It is
// intended to be used in evaluation the conditions that determine the scope of
// the parameter settings.
int Settings::getPropertyVal(Property prop)
{
    switch(prop) {
        case dflt:
            // dflt should be filtered out by caller
            std::cout << "Error: invalid property\n";
            exit(202);
        case current_period:
            return period;
        case gov_exp:
            return Government::Instance()->getExpenditure();
        case bens_paid:
            return Government::Instance()->getBenefitsPaid();
        case gov_recpts:
            return Government::Instance()->getReceipts();
        case deficit:
        {
            Government *gov = Government::Instance();
            return gov->getExpenditure() - gov->getReceipts();
        }
        case gov_bal:
            return Government::Instance()->getBalance();
        case num_firms:
            return int(reg->getNumFirms());
        case num_emps:
            return reg->getNumEmployed();
        case num_unemps:
            return reg->getNumUnemployed();
        case num_gov_emps:
            return int(Government::Instance()->getNumEmployees());
        case num_hired:
            return reg->getNumHired();
        case num_fired:
            return reg->getNumFired();
        case prod_bal:
            return reg->getProdBal();
        case wages:
            return reg->getWagesPaid();         // i.e. total for period, not rate
        case consumption:
            return reg->getPurchasesMade();
        case bonuses:
            return reg->getBonusesPaid();
        case dedns:
            return reg->getDednsPaid();
        case inc_tax:
            return reg->getIncTaxPaid();
        case sales_tax:
            return reg->getSalesTaxPaid();
        case dom_bal:
            return reg->getWorkersBal();
    }
}

int Settings::getParameterVal(ParamType type, int dflt)
{
    int res = dflt;
    for (auto it : cond) {
        if (applies(it->condition)) {
            Pair &p = (type == pt_iters) ? it->iters
            : ((type == pt_count) ? it->count
               : ((type == pt_emp_rate) ? it->emp_rate
                  : ((type == pt_std_wage) ? it->std_wage
                     : ((type == pt_prop_con) ? it->prop_con
                        : ((type == pt_inc_tax_rate) ? it->inc_tax_rate
                           : ((type == pt_sales_tax_rate) ? it->sales_tax_rate
                              : ((type == pt_firm_creation_prob) ? it->firm_creation_prob
                                 : ((type == pt_dedns) ? it->dedns
                                    : ((type == pt_unemp_ben_rate) ? it->unemp_ben_rate
                                       : ((type == pt_active_pop) ? it->active_pop
                                          : ((type == pt_reserve) ? it->reserve
                                             : ((type == pt_prop_inv) ? it->prop_inv
                                                : it->invalid
                                                )
                                             )
                                          )
                                       )
                                    )
                                 )
                              )
                           )
                        )
                     )
                  )
               );
            if (p.is_set) {
                res = p.val;
            }
        }
    }
    return res;
}

int Settings::getStdWage()
{
    return getParameterVal(pt_std_wage, 500);
}

int Settings::getPropCon()
{
    return getParameterVal(pt_prop_con, 80);
}

int Settings::getIncTaxRate()
{
    if (Settings::period > 5) {
        int x = 3;
    }
    return getParameterVal(pt_inc_tax_rate, 10);
}

int Settings::getSalesTaxRate()
{
    return getParameterVal(pt_sales_tax_rate, 15);
}

int Settings::getPreTaxDedns()
{
    return getParameterVal(pt_dedns, 10);
}

int Settings::getFCP()
{
    return getParameterVal(pt_firm_creation_prob, 10);
}

int Settings:: getUBR()
{
    return getParameterVal(pt_unemp_ben_rate, 60);
}

int Settings::getReserve()
{
    return getParameterVal(pt_reserve, 80);
}

int Settings::getPropInv()
{
    return getParameterVal(pt_prop_inv, 80);
}

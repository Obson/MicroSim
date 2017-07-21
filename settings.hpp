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
#include <map>
#include "account.hpp"
#include "register.hpp"

class Settings
{
private:
    
    static Settings *_instance;
    static int id;
    
    Register *reg;
    
protected:
    
    enum Property
    {
        dflt,
        current_period,
        gov_exp,
        bens_paid,
        gov_recpts,
        deficit,
        gov_bal,
        num_firms,
        num_emps,
        num_unemps,
        num_gov_emps,
        num_hired,
        num_fired,
        prod_bal,
        wages,
        consumption,
        bonuses,
        dedns,
        inc_tax,
        sales_tax,
        dom_bal
    };
    
    enum Opr
    {
        invalid_op,
        eq,
        neq,
        lt,
        gt,
        leq,
        geq
    };
    
    struct Condition
    {
        Property property = dflt;
        Opr opr;
        int val;                // possibly extend to allow expressions later
    };
    
    struct Pair
    {
        bool is_set;
        int val;
    };
    
    struct Params
    {
        Condition condition;
        Pair invalid;
        Pair iters;
        Pair count;
        Pair emp_rate;
        Pair std_wage;
        Pair prop_con;
        Pair inc_tax_rate;
        Pair sales_tax_rate;
        Pair firm_creation_prob;
        Pair dedns;
        Pair unemp_ben_rate;
        Pair active_pop;
        Pair reserve;
        Pair prop_inv;
    };
    
    std::vector<Params*> cond;
    
    static size_t parseLine(std::string &input, std::vector<std::string> &output);
    static bool validatePercent(int n, const std::string &descr, int min = 0, int max = 100);
    
    Settings();

    Property parsePropertyName(std::string);
    Opr parseOperator(std::string);
    
    bool applies(Condition&);
    bool compare(int lhs, int rhs, Opr op);
    
    enum ParamType {
        pt_iters,
        pt_count,
        pt_emp_rate,
        pt_std_wage,
        pt_prop_con,
        pt_inc_tax_rate,
        pt_sales_tax_rate,
        pt_firm_creation_prob,
        pt_dedns,
        pt_unemp_ben_rate,
        pt_active_pop,
        pt_reserve,
        pt_prop_inv
    };
    
    int getParameterVal(ParamType type, int dflt);
    
public:

    // The filename is ignored on all calls after the first
    static Settings *Instance();
    static std::string fname;
    
    static int period;
    
    int getPropertyVal(Property);
    
    int getIters();         // number of iterations (periods)
    int getPopSize();       // max available population size
    int getActivePop();     // target size of economically active population
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

//
//  statistics.hpp
//  Test Product
//
//  Created by David Brown on 10/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#ifndef statistics_hpp
#define statistics_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

class Statistics
{
private:
    
    static Statistics *_instance;

    struct fields
    {
        int num_firms = 0;
        int num_employed = 0;
        int num_hired = 0;
        int num_fired = 0;
        int num_rehired = 0;
        int num_unemployed = 0;

        int tax_recd = 0;               // recd by government

        int f_start_bal = 0;            // firms
        
        int start_bal = 0;              // workers only
        
        int dedns_paid = 0;             // paid by firms
        int wages_paid = 0;
        
        int w_start_bal_unemp = 0;        int w_end_bal_unemp = 0;
        
        int inc_tax_paid = 0;
        int sales_tax_paid = 0;
        
        int inc_tax_paid_unemp = 0;
        int sales_tax_paid_unemp = 0;
        
        int gov_grant = 0;
        int wages_recd = 0;
        
        int tot_sales = 0;
        int tot_purchases = 0;
        
        int tot_purch_unemp = 0;
        
        int gov_bal = 0;
        int gov_exp = 0;
        int prod_bal = 0;
        int house_bal = 0;
    };
    
    std::vector<fields*> stats;
    
protected:
    
    Statistics();
    
public:
    
    static Statistics *Instance();
    struct fields *current;
    void next(int period);
    void report();
};

#endif /* statistics_hpp */

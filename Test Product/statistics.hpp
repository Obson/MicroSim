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

        int tot_dedns = 0;
        int tot_inc_tax = 0;
        int tot_sales_tax = 0;
        
        int tot_sales = 0;
        
        int gov_bal = 0;
        int prod_bal = 0;
        int house_bal = 0;
    };
    
    std::vector<fields*> stats;
    
    struct fields *current;
    
protected:
    
    Statistics();
    
public:
    
    static Statistics *Instance();
    void next(int period);
    void report();
};

#endif /* statistics_hpp */

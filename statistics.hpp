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

#include "account.hpp"

class Statistics
{
private:
    
    static Statistics *_instance;

    struct Fields
    {
        struct
        {
            int bfwd = 0;
            int exp = 0;
            int ben = 0;
            int rec = 0;
            int cfwd = 0;
            //int def = 0;
            size_t num_firms = 0;
            size_t num_emps = 0;
            int hires = 0;
            int fires = 0;
        } gov;
        
        struct
        {
            int bfwd = 0;
            int grant = 0;
            int sales = 0;
            int sales_tax = 0;
            int dedns = 0;
            int wages = 0;
            int bonuses = 0;
            int cfwd = 0;
        } prod;
        
        struct
        {
            int start = 0;      // NB not bfwd as may have changed status
            int wages = 0;
            int benefits = 0;   // may have been unemployed at start of period
            int inc_tax = 0;
            int purch = 0;
            int sales_tax = 0;
            int close = 0;
        } emp;
        
    };
    
    std::vector<Fields*> stats;
    
protected:
    
    Statistics();
    
public:
    
    static Statistics *Instance();
    struct Fields *current;
    void next(int period);
    void report();
};

#endif /* statistics_hpp */

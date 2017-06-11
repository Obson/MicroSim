//
//  statistics.cpp
//  Test Product
//
//  Created by David Brown on 10/06/2017.
//  Copyright © 2017 David Brown. All rights reserved.
//

#include "statistics.hpp"

Statistics *Statistics::_instance = nullptr;

Statistics *Statistics::Instance()
{
    if (_instance == nullptr) {
        _instance = new Statistics();
    }
    return _instance;
}

Statistics::Statistics()
{
    current = new fields;
    *current = {};
}

void Statistics::next(int period)
{
    stats.push_back(current);
    current = new fields;
    *current = {};
}

void Statistics::report()
{
    std::cout << "\nStatistics\n";

    int period = 0;
    for (auto it : stats) {
        std::cout << "\nPeriod " << ++period << "\n";
    }
}

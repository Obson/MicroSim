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
    if (_instance == nullptr) {
        _instance = new Settings();
        _instance->prop_con = 0;
        _instance->inc_tax = 0;
        _instance->sales_tax = 0;
    }
    return _instance;
}

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
    }
    return _instance;
}

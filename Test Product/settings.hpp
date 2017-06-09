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

class Settings
{
private:
    
    static Settings *_instance;
    
public:
    
    static Settings *Instance();
    
    // Percentages
    int prop_con;
    int inc_tax;
    int sales_tax;
    int dedns;
   
};

#endif /* settings_hpp */

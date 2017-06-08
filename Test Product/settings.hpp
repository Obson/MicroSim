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
    
    int prop_con;
    
};

#endif /* settings_hpp */

//
//  Utils.cpp
//  dalipoker
//
//  Created by lugan on 7/19/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#include "Utils.h"

void LOGI (const char* msg, ...) {
    char buffer[1024] = {};
    va_list args;
    va_start(args, msg);
    vsprintf(buffer,msg,args);
    va_end(args);
    
//    sprintf(buffer, msg, args);
    cout << buffer << std::endl;
}
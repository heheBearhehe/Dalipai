//
//  Utils.cpp
//  dalipoker
//
//  Created by lugan on 7/19/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#include "Utils.h"

static bool sEnableLog = true;

void initUtils(){
    srand((unsigned)time(NULL));
}

void enableLog(bool enable){
    sEnableLog = enable;
}

void LOGI (const char* msg, ...) {
    if (!sEnableLog) {
        return;
    }
    
    char buffer[1024] = {};
    va_list args;
    va_start(args, msg);
    vsprintf(buffer,msg,args);
    va_end(args);
    cout << "dali:" << buffer << std::endl;
}

void LOGF (const char* msg, ...) {
    char buffer[1024] = {};
    va_list args;
    va_start(args, msg);
    vsprintf(buffer,msg,args);
    va_end(args);
    cout << buffer << std::endl;
}

bool getRandomSelect(int prob){
    return prob > getRandomProb();
}

int getRandomProb(){
    return RANDOM_MIN + rand() % (RANDOM_MAX - RANDOM_MIN);
}

int getRandom(int max){
    return rand() % max;
}





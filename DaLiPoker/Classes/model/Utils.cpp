//
//  Utils.cpp
//  dalipoker
//
//  Created by lugan on 7/19/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#include "Utils.h"
#include <sys/time.h>
#include <iomanip>

static bool sEnableLog = true;

void initUtils(){
    srand((unsigned)time(NULL));
}

void enableLog(bool enable){
    sEnableLog = enable;
}

static double START_TIME = 0;
double getTimestamp(){
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    double cur = (long)((double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000);
    if (START_TIME == 0) {
        START_TIME = cur;
    }
    
    return (cur - START_TIME) / 1000.0;
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
    
    cout.setf(std::ios::fixed);
    cout << "dali:" << "#" << std::setprecision(3) << getTimestamp() << "#  " << buffer << std::endl;
}

void LOGF (const char* msg, ...) {
    char buffer[1024] = {};
    va_list args;
    va_start(args, msg);
    vsprintf(buffer,msg,args);
    va_end(args);
    
    cout.setf(std::ios::fixed);
    cout << "#" << std::setprecision(3) << getTimestamp() << "#  " << buffer << std::endl;
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





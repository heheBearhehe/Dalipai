//
//  Utils.h
//  dalipoker
//
//  Created by lugan on 7/19/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#ifndef __dalipoker__Utils__
#define __dalipoker__Utils__

#include <stdio.h>
#include <stdarg.h>

#include <iostream>
using std::cout;


#define RANDOM_MAX 100
#define RANDOM_MIN 0

void initUtils();

void enableLog(bool enable);
    
void LOGI (const char* msg, ...) ;

void LOGF (const char* msg, ...) ;

bool getRandomSelect(int prob); // RANDOM_MIN - RANDOM_MAX

int getRandomProb();

int getRandom(int max);

#endif /* defined(__dalipoker__Utils__) */

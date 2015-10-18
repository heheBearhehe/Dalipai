//
//  Settings.cpp
//  DaLiPoker
//
//  Created by lugan on 10/18/15.
//
//

#include "Settings.h"

Settings* Settings::sInstance = NULL;

Settings* Settings::getInstance(){
    if (sInstance == NULL) {
        sInstance = new Settings();
    }
    return sInstance;
}

void Settings::destroy(){
    delete sInstance;
    sInstance = NULL;
}

Settings::Settings(){
    card1Weight = -1;
    card2Weight = -1;
    card3Weight = -1;
    giveProb = -1;
}
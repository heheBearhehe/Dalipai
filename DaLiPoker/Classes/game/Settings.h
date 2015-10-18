//
//  Settings.h
//  DaLiPoker
//
//  Created by lugan on 10/18/15.
//
//

#ifndef Settings_h
#define Settings_h

#include <stdio.h>

class Settings
{
public:
    static Settings* getInstance();
    static void destroy();
    
public:
    int card1Weight;
    int card2Weight;
    int card3Weight;
    int giveProb;
    
private:
    Settings();
    static Settings* sInstance;
};


#endif /* Settings_h */

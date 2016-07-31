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


static const std::string SETTINGS_FILENAME  = "settings.json";


class Settings
{
public:
    static Settings* getInstance();
    static void destroy();
    
    void load();
    void save();
    
public:
    int opponentCharacter;
    int myAvatar;
    int firstPlayer;
    bool backgroundMusic;
    bool soundEffect;
    
private:
    Settings();
    static Settings* sInstance;
};


#endif /* Settings_h */

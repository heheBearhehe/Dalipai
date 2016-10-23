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
#include "GameManager.h"
#include "GameStat.h"


static const std::string SETTINGS_FILENAME  = "settings.json";
static const std::string STATISTIC_FILENAME  = "stat.json";

static const int CHARACTOR_TOTAL         = 0;
static const int CHARACTOR_TOTAL_COUNT   = CHARACTOR_COUNT;

class Settings
{
public:
    static Settings* getInstance();
    static void destroy();
    
    void load();
    void save();
    
    void loadAllStat();
    void saveAllStat();
    
    void loadStat(int charactor);
    void saveStat(int charactor);
    
    void addResult(int charactor, GameResult* gameResult);
    GameStat& getGameStat(int charactor);
    
private:
    std::string getStatFilePath(int charactor);
    
public:
    int opponentCharacter;
    int myAvatar;
    int firstPlayer;
    bool backgroundMusic;
    bool soundEffect;
    
    GameStat totalStatForCharacter[CHARACTOR_TOTAL_COUNT];
    vector<GameResult *>* totalResultList[CHARACTOR_TOTAL_COUNT];
    
private:
    Settings();
    static Settings* sInstance;
};


#endif /* Settings_h */

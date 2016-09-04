//
//  GameStat.hpp
//  DaLiPoker
//
//  Created by lugan on 8/28/16.
//
//

#ifndef GameStat_h
#define GameStat_h

#include <stdio.h>
#include <vector>

using std::vector;
using std::string;


typedef enum game_result{
    DRAW = 0,
    WIN,
    LOSE,
}GAME_RESULT;

class GameResult{
public:
    int result;
    int myMaxCombo;
    int myPoints;
    int oppoMaxCombo;
    int oppoPoints;
    
    void updateResult();
};

class GameStat{
public:
    GameStat();
    void reset();
    void calcStat(vector<GameResult *>* resultList);
    
    int totalGameCount;
    int totalWinCount;
    int totalDrawCount;
    int totalLoseCount;
    
    float winRatio;
    float drawRatio;
    float loseRatio;
    
    float winRatio10;
    float winRatio20;
    float winRatio50;
    
    int topPoints;
    int topMyComboPoints;
    int topOppoComboPoints;
    int maxComboWin;
    
};

#endif /* GameStat_h */

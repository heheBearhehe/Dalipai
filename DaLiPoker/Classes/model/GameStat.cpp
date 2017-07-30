//
//  GameStat.cpp
//  DaLiPoker
//
//  Created by lugan on 8/28/16.
//
//

#include "GameStat.h"

void GameResult::updateResult(){
    if (this->myPoints > this->oppoPoints) {
        this->result = GAME_RESULT::WIN;
    }else if(this->myPoints < this->oppoPoints){
        this->result = GAME_RESULT::LOSE;
    }else{
        this->result = GAME_RESULT::DRAW;
    }
}

GameStat::GameStat(){
    reset();
}

void GameStat::reset(){
    totalGameCount = 0;
    totalWinCount = 0;
    totalDrawCount = 0;
    totalLoseCount = 0;
    
    winRatio = 0;
    drawRatio = 0;
    loseRatio = 0;
    
    winRatio10 = -1;
    winRatio20 = -1;
    winRatio50 = -1;
    
    topPoints = 0;
    topMyComboPoints = 0;
    topOppoComboPoints = 0;
    maxComboWin = 0;
}

void GameStat::calcStat(vector<GameResult *>* resultList){
    reset();
    
    int currentWinCombo = 0;
    int mLastWinCount50 = 0;
    int mLastWinCount20 = 0;
    int mLastWinCount10 = 0;
    
    this->totalGameCount = (int)resultList->size();
    for(int i = 0; i < resultList->size(); i++){
        GameResult* gameResult = resultList->at(i);
        
        if (gameResult->result == GAME_RESULT::WIN) {
            this->totalWinCount++;
            currentWinCombo++;
            if (currentWinCombo > this->maxComboWin) {
                this->maxComboWin = currentWinCombo;
            }
            
            if (this->totalGameCount >= 50 && this->totalGameCount - i <= 50) {
                mLastWinCount50++;
            }
            
            if(this->totalGameCount >= 20 && this->totalGameCount - i <= 20){
                mLastWinCount20++;
            }
            
            if(this->totalGameCount >= 10 && this->totalGameCount - i <= 10){
                mLastWinCount10++;
            }
            
        }else{
            currentWinCombo = 0;
            if (gameResult->result == GAME_RESULT::DRAW) {
                this->totalDrawCount++;
            }else{
                this->totalLoseCount++;
            }
        }
        
        if (gameResult->myPoints > this->topPoints) {
            this->topPoints = gameResult->myPoints;
        }
        
        if (gameResult->myMaxCombo > this->topMyComboPoints) {
            this->topMyComboPoints = gameResult->myMaxCombo;
        }
        if (gameResult->oppoMaxCombo > this->topOppoComboPoints) {
            this->topOppoComboPoints = gameResult->oppoMaxCombo;
        }
    }
    if (currentWinCombo > this->maxComboWin) {
        this->maxComboWin = currentWinCombo;
    }
    
    if (this->totalGameCount > 0) {
        this->winRatio = (float)this->totalWinCount / this->totalGameCount;
        this->drawRatio = (float)this->totalDrawCount / this->totalGameCount;
        this->loseRatio = (float)this->totalLoseCount / this->totalGameCount;
    }
    
    if (this->totalGameCount >= 10) {
        this->winRatio10 = (float)mLastWinCount10 / 10;
    }
    if (this->totalGameCount >= 20) {
        this->winRatio20 = (float)mLastWinCount20 / 20;
    }
    if (this->totalGameCount >= 50) {
        this->winRatio50 = (float)mLastWinCount50 / 50;
    }
}




//
//  AIPlayer.h
//  dalipoker
//
//  Created by lugan on 7/19/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#ifndef __dalipoker__AIPlayer__
#define __dalipoker__AIPlayer__

#include "Game.h"
class Game;
class Player;

class AIPlayer : public PlayerChoiceListener
{
public:
    AIPlayer(Game* game);
    virtual ~AIPlayer();
    virtual int makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback);
    virtual bool onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard);
    
    void setGame(Game* game) { mGame = game; mGiveCount = 0; };
    void setStrategy(int strategy) { mStrategy = strategy; };
    int getStategy() { return mStrategy; }
    void setKeepStrategyWeight(int weight[]) { mKeepStrategyWeight = weight; }
    int* getKeepStrategyWeight() { return mKeepStrategyWeight; }
    
    void setGiveStrategy(int strategy){ mGiveStrategy = strategy; }
    int getGiveStrategy() { return mGiveStrategy; }
    int getGiveCount() { return mGiveCount; }
    
protected:
    int makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback, int strategy);
private:
    Game* mGame;
    int mStrategy;
    int* mKeepStrategyWeight;
    int mGiveStrategy;
    int mGiveCount;
};

#endif /* defined(__dalipoker__AIPlayer__) */

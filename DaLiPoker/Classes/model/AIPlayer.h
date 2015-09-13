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
    
private:
    Game* mGame;
};

#endif /* defined(__dalipoker__AIPlayer__) */

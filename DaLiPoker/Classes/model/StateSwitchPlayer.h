//
//  StateSwitchPlayer.h
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#ifndef __DaLiPoker__StateSwitchPlayer__
#define __DaLiPoker__StateSwitchPlayer__

#include <stdio.h>

#include "StateBase.h"

class Game;
class Player;

class StateSwitchPlayer : public StateBase
{
    public:
    StateSwitchPlayer(Game* game);
    virtual ~StateSwitchPlayer();
    virtual bool enter();
    virtual bool execute();
    virtual bool next();
};

#endif /* defined(__DaLiPoker__StateSwitchPlayer__) */

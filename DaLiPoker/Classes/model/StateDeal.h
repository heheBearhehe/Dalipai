//
//  StateDeal.h
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#ifndef __DaLiPoker__StateDeal__
#define __DaLiPoker__StateDeal__

#include <stdio.h>

#include "StateBase.h"

class Game;
class Player;

class StateDeal : public StateBase
{
    public:
    StateDeal(Game* game);
    virtual ~StateDeal();
    virtual bool enter();
    virtual bool execute();
    virtual bool next();
};

#endif /* defined(__DaLiPoker__StateDeal__) */

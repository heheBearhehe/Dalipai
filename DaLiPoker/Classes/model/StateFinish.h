//
//  StateFinish.h
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#ifndef __DaLiPoker__StateFinish__
#define __DaLiPoker__StateFinish__

#include <stdio.h>

#include "StateBase.h"

class Game;
class Player;

class StateFinish : public StateBase
{
    public:
    StateFinish(Game* game);
    virtual ~StateFinish();
    virtual bool enter();
    virtual bool execute();
    virtual bool next();
};

#endif /* defined(__DaLiPoker__StateFinish__) */

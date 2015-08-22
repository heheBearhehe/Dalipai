//
//  StatePlayerChoice.h
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#ifndef __DaLiPoker__StatePlayerChoice__
#define __DaLiPoker__StatePlayerChoice__

#include <stdio.h>

#include "StateBase.h"

class Game;
class Player;

class StatePlayerChoice : public StateBase
{
public:
    StatePlayerChoice(Game* game);
    virtual ~StatePlayerChoice();
    virtual bool enter();
    virtual bool execute();
    virtual bool next();

private:
    int mAvailableAction;
};

#endif /* defined(__DaLiPoker__StatePlayerChoice__) */

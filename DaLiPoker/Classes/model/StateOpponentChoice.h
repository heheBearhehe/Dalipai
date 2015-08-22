//
//  StateOpponentChoice.h
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#ifndef __DaLiPoker__StateOpponentChoice__
#define __DaLiPoker__StateOpponentChoice__

#include <stdio.h>

#include "StateBase.h"

class Game;
class Player;

class StateOpponentChoice : public StateBase
{
public:
    StateOpponentChoice(Game* game);
    virtual ~StateOpponentChoice();
    virtual bool enter();
    virtual bool execute();
    virtual bool next();

private:
    int mAvailableAction;
};

#endif /* defined(__DaLiPoker__StateOpponentChoice__) */

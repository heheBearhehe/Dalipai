//
//  StateBase.h
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#ifndef __DaLiPoker__StateBase__
#define __DaLiPoker__StateBase__

#include <stdio.h>

class Game;

class StateBase
{
public:
    StateBase(Game* game);
    virtual ~StateBase() = 0;
    
    virtual bool enter();
    virtual bool execute();
    virtual bool next() = 0;
    
protected:
    Game* mGame;
    bool mExecuted;
};


#endif /* defined(__DaLiPoker__StateBase__) */

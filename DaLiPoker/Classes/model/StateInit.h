//
//  StateInit.h
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#ifndef __DaLiPoker__StateInit__
#define __DaLiPoker__StateInit__

#include "StateBase.h"

class Game;
class Player;

class StateInit : public StateBase
{
public:
    StateInit(Game* game);
    virtual ~StateInit();
    virtual bool enter();
    virtual bool execute();
    virtual bool next();
};


#endif /* defined(__DaLiPoker__StateInit__) */

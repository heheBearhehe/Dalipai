//
//  StateFinish.cpp
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#include "StateFinish.h"
#include "Game.h"

StateFinish::StateFinish(Game* game):StateBase(game){
    
}

StateFinish::~StateFinish(){
    
}

bool StateFinish::enter(){
    return StateBase::enter();
}

bool StateFinish::execute(){
    mGame->onFinished();
    return StateBase::execute();
}

bool StateFinish::next(){
    return false;
}

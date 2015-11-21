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
    LOGI("******* StateFinish::enter()");
    return StateBase::enter();
}

bool StateFinish::execute(){
    LOGI("******* StateFinish::execute()");
    mGame->onFinished();
    return StateBase::execute();
}

bool StateFinish::next(){
    LOGI("******* StateFinish::next()");
    return false;
}

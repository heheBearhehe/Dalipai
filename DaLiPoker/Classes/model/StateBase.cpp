//
//  StateBase.cpp
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#include "StateBase.h"


StateBase::StateBase(Game* game):mGame(game){
    
}

StateBase::~StateBase(){
    
}

bool StateBase::enter(){
    mExecuted = false;
    return true;
}

bool StateBase::execute(){
    mExecuted = true;
    return true;
}
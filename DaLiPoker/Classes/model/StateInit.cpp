//
//  StateInit.cpp
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#include "StateInit.h"
#include "Game.h"

StateInit::StateInit(Game* game):StateBase(game){
    
}

StateInit::~StateInit(){
    
}

bool StateInit::enter(){
    LOGI("******* StateInit::enter()");
    return StateBase::enter();
}

bool StateInit::execute(){
    LOGI("******* StateInit::execute()");
    mGame->init();
    return StateBase::execute();
}

bool StateInit::next(){
    LOGI("******* StateInit::next()");
    mGame->nextState(STATE::PLAYER_DEAL);
    
    return true;
}
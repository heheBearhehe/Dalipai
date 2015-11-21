//
//  StateDeal.cpp
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#include "StateDeal.h"
#include "Game.h"

StateDeal::StateDeal(Game* game):StateBase(game){
    
}

StateDeal::~StateDeal(){
    
}

bool StateDeal::enter(){
    LOGI("******* StateDeal::enter()");
    return StateBase::enter();
}

bool StateDeal::execute(){
    LOGI("******* StateDeal::execute()");
    if (mGame->mCurrentPlayer == NULL) {
        LOGI("*** deal. no player");
        return false;
    }
    
    bool ret = StateBase::execute();
    Card* card = mGame->currentCard();
    if (card != NULL) {
        mGame->onActionExecuted(ACTION_START_GAME_STATE + STATE::PLAYER_DEAL, mGame->mCurrentPlayer, card, NULL);
    }
    
    return ret;
}

bool StateDeal::next(){
    LOGI("******* StateDeal::next()");
    mGame->nextState(STATE::PLAYER_CHOICE);
    return true;
}

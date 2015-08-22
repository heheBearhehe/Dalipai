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
    return StateBase::enter();
}

bool StateDeal::execute(){
    if (mGame->mCurrentPlayer == NULL) {
        LOGI("*** deal. no player");
        return false;
    }
    
    Card* card = mGame->currentCard();
    if (card != NULL) {
        mGame->onActionExecuted(STATE::PLAYER_DEAL, mGame->mCurrentPlayer, card, NULL);
    }
    
    return StateBase::execute();
}

bool StateDeal::next(){
    mGame->nextState(STATE::PLAYER_CHOICE);
    return true;
}

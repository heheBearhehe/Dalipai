//
//  StateSwitchPlayer.cpp
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#include "StateSwitchPlayer.h"
#include "Game.h"

StateSwitchPlayer::StateSwitchPlayer(Game* game):StateBase(game){
    
}

StateSwitchPlayer::~StateSwitchPlayer(){
    
}

bool StateSwitchPlayer::enter(){
    LOGI("******* StateSwitchPlayer::enter()");
    return StateBase::enter();
}

bool StateSwitchPlayer::execute(){
    LOGI("******* StateSwitchPlayer::execute()");
    mGame->switchPlayer();
    
    bool ret = StateBase::execute();
    mGame->onActionExecuted(0, mGame->currentPlayer(), mGame->currentCard(), NULL);
    return ret;
}

bool StateSwitchPlayer::next(){
    LOGI("******* StateSwitchPlayer::next()");
    if (!mExecuted) {
        return true;
    }
    
    ++mGame->mCurrentCardIndex;
    if (mGame->mCurrentCardIndex >= mGame->mCardList->size()) {
        mGame->nextState(STATE::FINISH);
    }else{
        mGame->nextState(STATE::PLAYER_DEAL);
    }
    
    return true;
}

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
    return StateBase::enter();
}

bool StateSwitchPlayer::execute(){
    mGame->switchPlayer();
    mGame->onActionExecuted(0, mGame->currentPlayer(), mGame->currentCard(), NULL);
    return StateBase::execute();
}

bool StateSwitchPlayer::next(){
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

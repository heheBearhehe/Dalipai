//
//  StateOpponentChoice.cpp
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#include "StateOpponentChoice.h"
#include "Game.h"
#include "Player.h"
#include "Card.h"
#include <cmath>

StateOpponentChoice::StateOpponentChoice(Game* game):StateBase(game){
    mAvailableAction = Player::PLAYER_CHOICE_AUTO;
}

StateOpponentChoice::~StateOpponentChoice(){
    
}

bool StateOpponentChoice::enter(){
    Player* oppenentPlayer = mGame->oppenentPlayer();
    
    Card* card = mGame->currentCard();
    Card* lastCard = oppenentPlayer->getLastCard();
    int action = 0;
    
    LOGI("%s# give=[%s]", oppenentPlayer->getDumpPrefix().c_str(), card->getDisplay().c_str());
    
    if(std::abs(float(lastCard->getRank()) - card->getRank()) <= 1){
        action = Player::PLAYER_CHOICE_REMOVE_FOR_GIVE;
    }else{
        action = Player::PLAYER_CHOICE_KEEP_FOR_GIVE;
    }
    mAvailableAction = action;
    mGame->mPlayerAction = action;
//    mGame->onPlayerAction(oppenentPlayer, action);
    
    action = oppenentPlayer->mChoiceListener->makeChoice(oppenentPlayer, card, action, oppenentPlayer->mCallback);
    
    LOGI("%s# action=[0x%x]", oppenentPlayer->getDumpPrefix().c_str(), action);
    StateBase::enter();
    return action > 0;
}

bool StateOpponentChoice::execute(){
    int action = mGame->mPlayerAction;
    if ((action & mAvailableAction) < 0) {
        return false;
    }
    
    Player* oppenentPlayer = mGame->oppenentPlayer();
    Card* currentCard = mGame->currentCard();
    Card* lastCard = NULL;
    switch (action) {
    case Player::PLAYER_CHOICE_KEEP_FOR_GIVE:
        oppenentPlayer->addCard(currentCard);
        break;
        
    case Player::PLAYER_CHOICE_REMOVE_FOR_GIVE:
        lastCard = oppenentPlayer->removeLastCard();
        if (lastCard != NULL) {
            mGame->mDiscardCardList->push_back(lastCard);
        }
        mGame->mDiscardCardList->push_back(currentCard);
        break;
    default:
        break;
    }
    mGame->onActionExecuted(action, oppenentPlayer, currentCard, lastCard);
    
    return StateBase::execute();
}

bool StateOpponentChoice::next(){
    if (!mExecuted) {
        return true;
    }
    
    int action = mGame->mPlayerAction;
    if ((action & mAvailableAction) < 0) {
        return false;
    }
    
    switch (action) {
    case Player::PLAYER_CHOICE_KEEP_FOR_GIVE:
        mGame->nextState(STATE::PLAYER_SWITCH);
        break;
        
    case Player::PLAYER_CHOICE_REMOVE_FOR_GIVE:
        mGame->nextState(STATE::PLAYER_SWITCH);
        break;
    default:
        break;
    }
    
    return true;
}

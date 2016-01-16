//
//  StatePlayerChoice.cpp
//  DaLiPoker
//
//  Created by lugan on 8/22/15.
//
//

#include "StatePlayerChoice.h"
#include "Game.h"
#include "Player.h"
#include "Card.h"
#include <cmath>

StatePlayerChoice::StatePlayerChoice(Game* game):StateBase(game){
    mAvailableAction = Player::PLAYER_CHOICE_AUTO;
}

StatePlayerChoice::~StatePlayerChoice(){
    
}

bool StatePlayerChoice::enter(){
    LOGI("******* StatePlayerChoice::enter()");
    StateBase::enter();
    Player* currentPlayer = mGame->currentPlayer();
    
    Card* card = mGame->currentCard();
    Card* lastCard = currentPlayer->getLastCard();
    int action = 0;
    int choice = 0;
    LOGI("%s# Player.%d.deal  card=[%s]", currentPlayer->getDumpPrefix().c_str(), currentPlayer->getTag(),
         card->getDisplay().c_str());
    if (lastCard == NULL) {
        choice = Player::PLAYER_CHOICE_KEEP;
        mAvailableAction = choice;
        mGame->onPlayerAction(currentPlayer, choice);
//        mGame->onActionExecuted(Player::PLAYER_CHOICE_KEEP, currentPlayer, card, NULL);
    }else{
        if(std::abs(float(lastCard->getRank()) - card->getRank()) <= 1){
            action = Player::PLAYER_CHOICE_DISCARD | Player::PLAYER_CHOICE_GIVE;
        }else{
            action = Player::PLAYER_CHOICE_KEEP | Player::PLAYER_CHOICE_DISCARD | Player::PLAYER_CHOICE_GIVE;
        }
        mAvailableAction = action;
        
        LOGI("*******  %s# makeChoice   availableChoice=[0x%x]", currentPlayer->getDumpPrefix().c_str(), mAvailableAction);
        
        choice = currentPlayer->mChoiceListener->makeChoice(currentPlayer, card, mAvailableAction, currentPlayer->mCallback);
        if (choice >= 0) {
            mGame->onPlayerAction(currentPlayer, choice);
//            if (choice > 0) {
//                mGame->onActionExecuted(choice, currentPlayer, card, NULL);
//            }
        }
    }
    
    LOGI("%s# action=[0x%x]", currentPlayer->getDumpPrefix().c_str(), action);
    
    return choice > 0;
}

bool StatePlayerChoice::execute(){
    LOGI("******* StatePlayerChoice::execute() mPlayerAction=[%d] mAvailableAction=[0x%x]", mGame->mPlayerAction, mAvailableAction);
    int action = mGame->mPlayerAction;
    if ((action & mAvailableAction) < 0) {
        return false;
    }
    
    Player* currentPlayer = mGame->currentPlayer();
    Card* card = mGame->currentCard();
    
    switch (action) {
    case Player::PLAYER_CHOICE_KEEP:
        currentPlayer->addCard(card);
        break;
    case Player::PLAYER_CHOICE_DISCARD:
        mGame->mDiscardCardList->push_back(card);
        break;
    case Player::PLAYER_CHOICE_GIVE:
        break;
    default:
        return false;
    }
    
    bool ret = StateBase::execute();
    mGame->onActionExecuted(action, mGame->currentPlayer(), mGame->currentCard(), NULL);
    return ret;
}

bool StatePlayerChoice::next(){
    LOGI("******* StatePlayerChoice::next()  mExecuted=[%d] mPlayerAction=[%d] mAvailableAction=[0x%x]", mExecuted, mGame->mPlayerAction, mAvailableAction);
    if (!mExecuted) {
        return false;
    }
    
    int action = mGame->mPlayerAction;
    if ((action & mAvailableAction) < 0) {
        return false;
    }
    
    switch (action) {
    case Player::PLAYER_CHOICE_KEEP:
        mGame->nextState(STATE::PLAYER_SWITCH);
        break;
    case Player::PLAYER_CHOICE_DISCARD:
        mGame->nextState(STATE::PLAYER_SWITCH);
        break;
    case Player::PLAYER_CHOICE_GIVE:
        mGame->nextState(STATE::PLAYER_OPPENENT_CHOICE);
        break;
    default:
        break;
    }
    
    return true;
}



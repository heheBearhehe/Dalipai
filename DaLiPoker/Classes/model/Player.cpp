//
//  Player.cpp
//  dalipoker
//
//  Created by lugan on 7/19/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#include "Player.h"
#include "Game.h"
#include "Card.h"
#include <cmath>
#include "Utils.h"


Player::Player(){
    mKeepCardList = new vector<Card *>();
    mChoiceListener = NULL;
}

Player::~Player(){
    delete mKeepCardList;
}

string Player::getDumpPrefix(){
    return this->getTag() > 1? "                                                            " : "                   ";
}

void Player::deal(Card* card){
    Card* lastCard = this->getLastCard();
    int action = 0;
    LOGI("%s# Player.%d.deal  card=[%s]", this->getDumpPrefix().c_str(), this->getTag(),
         card->getDisplay().c_str());
    if (lastCard == NULL) {
        action = PLAYER_CHOICE_KEEP;
    }else{
        if(std::abs(float(lastCard->getRank()) - card->getRank()) <= 1){
            action = PLAYER_CHOICE_DISCARD | PLAYER_CHOICE_GIVE;
        }else{
            action = PLAYER_CHOICE_KEEP | PLAYER_CHOICE_DISCARD | PLAYER_CHOICE_GIVE;
        }
        
        action = this->makeChoice(card, action);
    }
    
    LOGI("%s# action=[0x%x]", this->getDumpPrefix().c_str(), action);
    
    if(mCallback != NULL && action > 0){
        if (mChoiceListener != NULL) {
            mChoiceListener->onChoiceMade(this, action, card, lastCard);
        }
        mCallback->onPlayerAction(this, action);
    }
}

void Player::give(Card* card){
    LOGI("%s# give=[%s]", getDumpPrefix().c_str(), card->getDisplay().c_str());
    Card* lastCard = this->getLastCard();
    int action;
    
    if(std::abs(float(lastCard->getRank()) - card->getRank()) <= 1){
        action = PLAYER_CHOICE_REMOVE_FOR_GIVE;
    }else{
        action = PLAYER_CHOICE_KEEP_FOR_GIVE;
    }
    action = this->makeChoice(card, action);
    
    LOGI("%s# action=[0x%x]", getDumpPrefix().c_str(), action);
    if(mCallback != NULL && action > 0){
        if (mChoiceListener != NULL) {
            mChoiceListener->onChoiceMade(this, action, card, lastCard);
        }
        if(mCallback != NULL){
            mCallback->onPlayerAction(this, action);
        }
    }
}

int Player::makeChoice(Card* card, int availableChoice){
    LOGI("%s# makeChoice   availableChoice=[0x%x]", getDumpPrefix().c_str(), availableChoice);
    int choice = 0;
    if (mChoiceListener != NULL) {
        choice = mChoiceListener->makeChoice(this, card, availableChoice, mCallback);
        if (choice >= 0) {
            return choice;
        }
    }
    
    if ((availableChoice & PLAYER_CHOICE_KEEP) > 0) {
        return PLAYER_CHOICE_KEEP;
    }else if ((availableChoice & PLAYER_CHOICE_GIVE) > 0){
        return PLAYER_CHOICE_GIVE;
    }else if ((availableChoice & PLAYER_CHOICE_REMOVE_FOR_GIVE) > 0){
        return PLAYER_CHOICE_REMOVE_FOR_GIVE;
    }else if ((availableChoice & PLAYER_CHOICE_KEEP_FOR_GIVE) > 0){
        return PLAYER_CHOICE_KEEP_FOR_GIVE;
    }else{
        LOGI("%s# ERROR makeChoice  card=[%s] availableChoice=[0x%x]", getDumpPrefix().c_str(), card, availableChoice);
    }
}

Card* Player::getLastCard(){
    if (mKeepCardList->size() > 0) {
        return (*mKeepCardList->rbegin());
    }
    
    return NULL;
}

void Player::addCard(Card* card){
    LOGI("%s# addCard   card=[%s]", getDumpPrefix().c_str(), card->getDisplay().c_str());
    mKeepCardList->push_back(card);
}

Card* Player::removeLastCard(){
    Card* card = NULL;
    if (mKeepCardList->size() > 0) {
        card = *mKeepCardList->rbegin();
        LOGI("%s# removeLastCard   card=[%s]", getDumpPrefix().c_str(), card->getDisplay().c_str());
        mKeepCardList->pop_back();
    }
    return card;
}

int Player::calcPoints(){
    if (mKeepCardList == NULL || mKeepCardList->size() == 0) {
        return 0;
    }
    
    int points = 0;
    for(int i = 0; i < mKeepCardList->size(); i++){
        if(i == 0 || i == mKeepCardList->size() - 1){
            points++;
        }else{
            Card* prev = mKeepCardList->at(i - 1);
            Card* cur  = mKeepCardList->at(i);
            Card* next = mKeepCardList->at(i + 1);
            if ((prev->getRank() < cur->getRank() && cur->getRank() < next->getRank())
                || (prev->getRank() > cur->getRank() && cur->getRank() > next->getRank())) {
                points++;
            }
        }
    }
    
    return points;
}





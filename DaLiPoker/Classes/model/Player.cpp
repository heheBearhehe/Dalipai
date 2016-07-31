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


void Player::reset(){
    mKeepCardList->clear();
    mPoints = 0;
}

string Player::getDumpPrefix(){
    return this->getTag() > 1? "                                                            " : "                   ";
}

Card* Player::getLastCard(){
    if (mKeepCardList->size() > 0) {
        return (*mKeepCardList->rbegin());
    }
    
    return NULL;
}

int Player::getTrend(){
    if (mKeepCardList->size() <= 1) {
        return PlayerTrend::NONE;
    }
    
    Card* lastCard = mKeepCardList->at(mKeepCardList->size() - 1);
    Card* last2Card = mKeepCardList->at(mKeepCardList->size() - 2);
    
    if (lastCard->getRank() < 2) {
        return PlayerTrend::UP;
    } else if (lastCard->getRank() > 10) {
        return PlayerTrend::DOWN;
    }
    
    return lastCard->getRank() > last2Card->getRank() ? PlayerTrend::UP : PlayerTrend::DOWN;
}

void Player::addCard(Card* card, int source){
    LOGI("%s# addCard   card=[%s]", getDumpPrefix().c_str(), card->getDisplay().c_str());
    card->setSource(source);
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





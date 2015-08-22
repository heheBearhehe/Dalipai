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





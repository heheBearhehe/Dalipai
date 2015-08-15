//
//  Card.cpp
//  DaLiPoker
//
//  Created by lugan on 7/12/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#include "Card.h"


Card::Card(int rank, int suit):mRank(rank), mSuit(suit){
}

Card::Card(int index){
    mRank = index / SUIT::COUNT;
    mSuit = index % SUIT::COUNT;
}

string Card::toString(){
    char buffer[1024] = {};
    sprintf(buffer, "Card[%s] [%d-%d] index=[%d]", getDisplay().c_str(), mRank, mSuit, getIndex());

    return string(buffer);
}

string Card::getDisplay(){
    char buffer[1024] = {};
    sprintf(buffer, "%s%s", SUIT_DISPLAY_LIST[mSuit].c_str(), RANK_DISPLAY_LIST[mRank].c_str());
    
    return string(buffer);
}

string Card::getDisplaySuit(){
    return SUIT_DISPLAY_LIST[mSuit];
}

string Card::getDisplayNum(){
    return RANK_DISPLAY_LIST[mRank];
}

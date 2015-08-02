//
//  Recorder.cpp
//  dalipoker
//
//  Created by lugan on 8/1/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#include "Recorder.h"
#include "Card.h"
#include "Utils.h"
#include <iostream>

using namespace std;

Recorder::Recorder(){
    mCardIndexList = NULL;
    mActionList = NULL;
}

Recorder::~Recorder(){
    delete mCardIndexList;
}


void Recorder::setCardList(vector<Card *>* cardList){
    mCardIndexList = new vector<int>();
    
    vector<Card *>::iterator iter;
    for (iter = cardList->begin(); iter != cardList->end(); iter++){
        mCardIndexList->push_back((*iter)->getIndex());
    }
}


void Recorder::addPlayerAction(int playerTag, int action){
    if (mActionList == NULL) {
        mActionList = new vector<int>();
    }
    int serializationAction = (playerTag << 8) | action;
    mActionList->push_back(serializationAction);
    LOGI("saved action=[0x%x]", serializationAction);
}



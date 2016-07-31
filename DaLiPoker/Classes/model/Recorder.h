//
//  Recorder.h
//  dalipoker
//
//  Created by lugan on 8/1/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#ifndef __dalipoker__Recorder__
#define __dalipoker__Recorder__

#include <stdio.h>

#include <stdio.h>
#include <vector>

using std::vector;
using std::string;

class Card;


class Recorder {
    
public:
    Recorder();
    ~Recorder();
    
    void setCardList(vector<Card *>* cardList);
    void setPlayerFirstPlay(bool isPlayer1) { mIsPlayer1FirstPlay = isPlayer1; };
    void addPlayerAction(int playerTag, int action);
    
    vector<int>* getCardIndexList() { return mCardIndexList; }
    vector<int>* getActionList() { return mActionList; }
    bool getIsPlayer1FirstPlay() { return mIsPlayer1FirstPlay; }
    
    int getAction(int index, int playerTag);
        
private:
    vector<int>* mCardIndexList;
    vector<int>* mActionList;
    bool mIsPlayer1FirstPlay;
    
};

#endif /* defined(__dalipoker__Recorder__) */

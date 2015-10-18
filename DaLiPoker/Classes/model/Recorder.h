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
    void addPlayerAction(int playerTag, int action);
    
    vector<int>* getCardIndexList() { return mCardIndexList; }
    vector<int>* getActionList() { return mActionList; }
    
    int getAction(int index, int playerTag);
        
private:
    vector<int>* mCardIndexList;
    vector<int>* mActionList;
    
};

#endif /* defined(__dalipoker__Recorder__) */

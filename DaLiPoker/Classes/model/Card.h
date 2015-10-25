//
//  Card.h
//  DaLiPoker
//
//  Created by lugan on 7/12/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#ifndef __DaLiPoker_Card__
#define __DaLiPoker_Card__

#include <stdio.h>
#include <string>

using std::string;

typedef enum type{
    SPADE = 0,
    HEART,
    DIAMOND,
    CLUB,
    COUNT
}SUIT;

static string SUIT_DISPLAY_LIST[] = {"♠️", "❤️", "♦️", "♣️"};
static string RANK_DISPLAY_LIST[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

class Card {
public:
    Card(int rank, int suit);
    Card(int index);
    ~Card();
    virtual string toString();

    int getRank() { return mRank; }
    int getSuit() { return mSuit; }
    int getIndex() { return mRank * SUIT::COUNT + mSuit; }
    int getTag() { return mTag; }
    void setTag(int tag) { mTag = tag; }
    int getSeq() { return mSeq; }
    void setSeq(int seq) { mSeq = seq; }
    int getScored() { return mScored; }
    void setScored(bool scored) { mScored = scored; }
    
    string getDisplay();
    string getDisplaySuit();
    string getDisplayNum();
    
private:
    int mRank;
    int mSuit;
    int mTag;
    int mSeq;
    int mScored;
};


#endif /* defined(__DaLiPoker_Card__) */

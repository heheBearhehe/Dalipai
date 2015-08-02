//
//  Game.cpp
//  DaLiPoker
//
//  Created by lugan on 7/12/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#include "Game.h"
#include "Card.h"
#include "Player.h"
#include "Recorder.h"
#include "Utils.h"

#include <iostream>
#include <random>
#include <algorithm>
using std::cout;


Game::Game(GAME_MODE mode){
    mPlayMode = PLAY_MODE::AUTO;
    mCardList = new vector<Card *>();
    mDiscardCardList = new vector<Card *>();
    mGameMode = mode;
    mState = STATE::NEW;
    mPlayer1 = NULL;
    mPlayer2 = NULL;
    mRecorder = NULL;
}

Game::~Game(){
    delete mRecorder;
    vector<Card *>::iterator iter;
    for (iter = mCardList->begin(); iter != mCardList->end(); iter++){
        delete (*iter);
    }
    delete mCardList;
    
    for (iter = mDiscardCardList->begin(); iter != mDiscardCardList->end(); iter++){
        delete (*iter);
    }
    delete mDiscardCardList;
}

bool Game::init(){
    if(mState != STATE::NEW){
        return false;
    }
    
    mState = STATE::INIT;
    
    if (mPlayMode == PLAY_MODE::AUTO) {
        initCards();
        shuffle();
        mRecorder = new Recorder();
        mRecorder->setCardList(mCardList);
    }else if(mPlayMode == PLAY_MODE::REPLAY){
        vector<Card *>::iterator iter;
        for (iter = mCardList->begin(); iter != mCardList->end(); iter++){
            delete (*iter);
        }
        delete mCardList;
        
        mCardList = new vector<Card *>();
        
        vector<int>* cardIndexList = mRecorder->getCardIndexList();
        vector<int>* actionList = mRecorder->getActionList();
        vector<int>::iterator iterIndex;
        for (iterIndex = cardIndexList->begin(); iterIndex != cardIndexList->end(); iterIndex++) {
            mCardList->push_back(new Card(*iterIndex));
        }
    }
    
    return true;
}

void Game::initCards(){
    int numMin = 1;
    int numMax = 13;
    if (mGameMode == GAME_MODE::SMALL) {
        numMin = 3;
        numMax = 11;
    }
    
    int numCount = numMax - numMin + 1;
    for (int i = 0; i < numCount * SUIT::COUNT; i++) {
        mCardList->push_back(new Card(i / SUIT::COUNT + numMin, i % SUIT::COUNT));
    }
}

void Game::dumpCards(){
    vector<Card *>::iterator iter;
    for (iter = mCardList->begin(); iter != mCardList->end(); iter++){
        LOGI((*iter)->toString().c_str());
    }
    LOGI("---");
}

void Game::shuffle(){
    std::default_random_engine gen((unsigned)time(NULL));
    std::shuffle(mCardList->begin(),mCardList->end(), gen);
}

bool Game::setPlayer(Player* player1, Player* player2){
    if (mState != STATE::INIT) {
        return false;
    }
    
    LOGI("- setPlayer");

    mPlayer1 = player1;
    mPlayer2 = player2;
    mCurrentPlayer = mPlayer1;
    mNextPlayer = player2;
    
    player1->setPlayerActionCallBack(this);
    player2->setPlayerActionCallBack(this);
    
    return true;
}

bool Game::start(){
    LOGI("- start");

    if(mState != STATE::INIT){
        LOGI("*** start. not init");
        return false;
    }
    
    if(mPlayer1 == NULL || mPlayer2 == NULL){
        LOGI("***  player NULL");
        return false;
    }
    
    mCurrentCardIndex = 0;
    deal();
    
    return true;
}

void Game::next(){
    LOGI("- next");
    ++mCurrentCardIndex;
    if (mCurrentCardIndex >= mCardList->size()) {
        onFinished();
    }else{
        Player* temp = mCurrentPlayer;
        mCurrentPlayer = mNextPlayer;
        mNextPlayer = temp;
        
        deal();
    }
}

void Game::onPlayerAction(Player* player, int action){
//    if (player != mCurrentPlayer && mCurrentPlayer != NULL) {
//        return;
//    }
    
    LOGI("- onPlayerAction. player=[%d]  action=[%d]", player->getTag(), action);
    
    if (mRecorder != NULL) {
        mRecorder->addPlayerAction(player->getTag(), action);
    }
    
    Card* card = mCardList->at(mCurrentCardIndex);
    switch (action) {
        case Player::PLAYER_CHOICE_KEEP:
            player->addCard(card);
            break;
        
        case Player::PLAYER_CHOICE_DISCARD:
            mDiscardCardList->push_back(card);
            break;
            
        case Player::PLAYER_CHOICE_GIVE:
            mNextPlayer->give(card);
            break;
            
        case Player::PLAYER_CHOICE_REMOVE:
            mNextPlayer->removeLastCard();
            break;
            
        default:
            break;
    }
    
    
    if (action != Player::PLAYER_CHOICE_GIVE) {
        next();
    }
}

void Game::deal(){
    LOGI("- deal");
    if (mCurrentPlayer == NULL) {
        LOGI("*** deal. no player");
        return;
    }
    
    if (mCurrentCardIndex >= 0 && mCurrentCardIndex < mCardList->size()) {
        Card* card = mCardList->at(mCurrentCardIndex);
        
        if (mPlayMode == PLAY_MODE::AUTO) {
            mCurrentPlayer->deal(card);
        }else{
//            this->onPlayerAction(mCurrentPlayer, action);
        }
    }else{
        LOGI("*** deal. error");
    }
}

void Game::pause(){
    
}

void Game::stop(){
    
}

void Game::onFinished(){
    LOGI("--------- onFinished");
    int p1Points = mPlayer1->getPoints();
    int p2Points = mPlayer2->getPoints();
    
    LOGI("--------- player1 count=[%d]  pts=[%d]", mPlayer1->getCardList()->size(), p1Points);
    vector<Card *>::iterator iter;
    for (iter= mPlayer1->getCardList()->begin(); iter!=mPlayer1->getCardList()->end(); iter++){
        LOGI((*iter)->getDisplay().c_str());
    }
    
    LOGI("--------- player2 count=[%d] pts=[%d]", mPlayer1->getCardList()->size(), p2Points);
    for (iter= mPlayer2->getCardList()->begin(); iter!=mPlayer2->getCardList()->end(); iter++){
        LOGI((*iter)->getDisplay().c_str());
    }
    
    LOGI("-- p1.pts=[%d]  p2.pts=[%d]", p1Points, p2Points);
}

void Game::replay(){
    mPlayMode = PLAY_MODE::REPLAY;
    init();
    start();
}





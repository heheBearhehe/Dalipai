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
#include "AIPlayer.h"

#include "StateInit.h"
#include "StateDeal.h"
#include "StatePlayerChoice.h"
#include "StateOpponentChoice.h"
#include "StateSwitchPlayer.h"
#include "StateFinish.h"

#include <iostream>
#include <random>
#include <algorithm>
using std::cout;


Game::Game(GAME_MODE mode, PLAY_MODE playMode){
    mPlayMode = playMode;
    mCardList = new vector<Card *>();
    mDiscardCardList = new vector<Card *>();
    mGameMode = mode;
    mState = STATE::INIT;
    mPlayer1 = NULL;
    mPlayer2 = NULL;
    mRecorder = NULL;
    mPlayerChoiceListener = NULL;
    mPlayer2ChoiceListener = NULL;
    mGameStateListener = NULL;
    
    mStateList = new vector<StateBase *>();
    mStateList->push_back(new StateInit(this));
    mStateList->push_back(new StateDeal(this));
    mStateList->push_back(new StatePlayerChoice(this));
    mStateList->push_back(new StateOpponentChoice(this));
    mStateList->push_back(new StateSwitchPlayer(this));
    mStateList->push_back(new StateFinish(this));
    
    mCurrentState = getState(STATE::INIT);
}

Game::~Game(){
    delete mRecorder;
    vector<Card *>::iterator iter;
    for (iter = mCardList->begin(); iter != mCardList->end(); iter++){
        delete (*iter);
    }
    delete mCardList;
    
    delete mDiscardCardList;
}

void Game::reset(){
    mPlayer1->reset();
    mPlayer2->reset();
    mCurrentPlayer = mPlayer1;
    mNextPlayer = mPlayer2;
    mState = STATE::INIT;
    mCurrentState = getState(STATE::INIT);
    mCurrentCardIndex = 0;
    mDiscardCardList->clear();
    
    if (mPlayerChoiceListener != NULL) {
        mPlayerChoiceListener->reset();
    }
    if (mPlayer2ChoiceListener != NULL) {
        mPlayer2ChoiceListener->reset();
    }
    
    mP1ActionCountTotal = 0;
    mP1GuessCountTotal = 0;
    mP1GuessCountCorrect = 0;
    mP1GuessCountScore = 0;
    mP2ActionCountTotal = 0;
    mP2GuessCountTotal = 0;
    mP2GuessCountCorrect = 0;
    mP2GuessCountScore = 0;
}

bool Game::init(){
    if (mPlayMode == PLAY_MODE::AUTO) {
        initCards();
        shuffle();
        mRecorder = new Recorder();
        mRecorder->setCardList(mCardList);
    }else if(mPlayMode == PLAY_MODE::REPLAY){
//        vector<Card *>::iterator iter;
//        for (iter = mCardList->begin(); iter != mCardList->end(); iter++){
//            delete (*iter);
//        }
//        delete mCardList;
        
        mCardList = new vector<Card *>();
        
        vector<int>* cardIndexList = mRecorder->getCardIndexList();
        vector<int>::iterator iterIndex;
        int seq = 0;
        for (iterIndex = cardIndexList->begin(); iterIndex != cardIndexList->end(); iterIndex++) {
            Card* card = new Card(*iterIndex);
            card->setTag(seq % 2 + 1);
            card->setSeq(seq++);
            mCardList->push_back(card);
        }
    }
    
    return true;
}

void Game::initCards(){
    enableLog(false);
    int numMin = 0;
    int numMax = 12;
    if (mGameMode == GAME_MODE::SMALL) {
        numMin = 2;
        numMax = 10;
    }
    
    int numCount = numMax - numMin + 1;
    for (int i = 0; i < numCount * SUIT::COUNT; i++) {
        Card* card = new Card(i / SUIT::COUNT + numMin, i % SUIT::COUNT);
        
//        // for test
//        if (card->getSuit() == SUIT::DIAMOND || card->getSuit() == SUIT::CLUB /*|| card->getSuit() == SUIT::HEART*/) {
//            delete card;
//            continue;
//        }
        
        mCardList->push_back(card);
    }
    
    mMinRank = numMin;
    mMaxRank = numMax;
    mCardCountToPlay = mCardList->size();
    
//    mCardCountToPlay = 12;
}

void Game::dumpCards(){
    vector<Card *>::iterator iter;
    for (iter = mCardList->begin(); iter != mCardList->end(); iter++){
        LOGI((*iter)->toString().c_str());
    }
    LOGI("---");
}

void Game::shuffle(){
    std::default_random_engine gen(std::chrono::system_clock::now ().time_since_epoch ().count ());
    std::shuffle(mCardList->begin(),mCardList->end(), gen);
    for (int i = 0; i < mCardList->size(); i++){
        mCardList->at(i)->setSeq(i);
    }
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
    if(mPlayer1 == NULL || mPlayer2 == NULL){
        LOGI("***  player NULL");
        return false;
    }
    
    if (mPlayer2ChoiceListener == NULL) {
        mPlayer2ChoiceListener = new AIPlayer(mMinRank, mMaxRank);
        mPlayer2->setChoiceListener(mPlayer2ChoiceListener);
    }
    
    reset();
    mCurrentState->execute();
    mCurrentState->next();
    
    return true;
}

void Game::next(){
    LOGI("******* game.next");
    mCurrentState->next();
}

void Game::execute(){
    LOGI("******* game.execute");
    mCurrentState->execute();
    if (mGameStateListener == NULL) {
        next();
    }
}

void Game::switchPlayer(){
    Player* temp = mCurrentPlayer;
    mCurrentPlayer = mNextPlayer;
    mNextPlayer = temp;
}

void Game::onPlayerAction(Player* player, int action){
    if (mCurrentCardIndex >= mCardList->size()) {
        return;
    }
    
    if (player == NULL) {
        player = mCurrentPlayer;
        if (player == NULL) {
            return;
        }
    }
    
    LOGI("******* game.onPlayerAction. player=[%d]  action=[%d]", player->getTag(), action);
    Card* card = currentCard();
    if (card != NULL && card->getTag() == 0) {
        card->setTag(player->getTag());
    }
    
    if (mPlayMode != PLAY_MODE::REPLAY && mRecorder != NULL && action > Player::PLAYER_CHOICE_AUTO && action <= Player::PLAYER_CHOICE_GIVE) {
        mRecorder->addPlayerAction(player->getTag(), action);
    }
    
    mPlayerAction = action;
    
    
    switch (action) {
        case Player::PLAYER_CHOICE_KEEP:
            break;
        
        case Player::PLAYER_CHOICE_DISCARD:
            break;
            
        case Player::PLAYER_CHOICE_GIVE:
            break;
            
        case Player::PLAYER_CHOICE_REMOVE_FOR_GIVE:
        {
            break;
        }
        case Player::PLAYER_CHOICE_KEEP_FOR_GIVE:
            break;
            
        default:
            break;
    }
    
}

void Game::pause(){
    
}

void Game::stop(){
    
}

void Game::onFinished(){
    mState = STATE::FINISH;
    if (mGameStateListener != NULL) {
        mGameStateListener->onFinished();
    }
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


int Game::getResetCardsCount(){
    return mCardList->size()- mCurrentCardIndex - 1;
}

vector<Card *>* Game::getOpponentCardsList(){
    return mPlayer2->getCardList();
}

int Game::getMyPlayerPoints(){
    return mPlayer1->getPoints();
}

int Game::getOpponentPoints(){
    if (mPlayMode != PLAY_MODE::REPLAY && mState != STATE::FINISH) {
        return 0;
    }
    
    return mPlayer2->getPoints();
}

vector<Card *>* Game::getMyPlayerCardList(){
    return mPlayer1->getCardList();
}

vector<Card *>* Game::getDiscardCardList(){
    return this->mDiscardCardList;
}

void Game::setPlayer1ChoiceListener(PlayerChoiceListener* l) {
    mPlayerChoiceListener = l;
    mPlayer1->setChoiceListener(l);
}

void Game::setPlayer2ChoiceListener(PlayerChoiceListener* l) {
    mPlayer2ChoiceListener = l;
    mPlayer2->setChoiceListener(l);
}


StateBase* Game::getState(STATE state){
    return mStateList->at(state);
}

void Game::nextState(STATE nextState){
    mCurrentState = getState(nextState);
    
    if(mCurrentState->enter()){
//        mCurrentState->execute();
        execute();
    }
}

void Game::onActionExecuted(int action, Player* player, Card* card1, Card* card2){
    if (mGameStateListener != NULL) {
        mGameStateListener->onActionExecuted(action, player, card1, card2);
    }
    if (action < ACTION_START_GAME_STATE && action > 0) {
        if (player->getTag() == 1) {
            mPlayerChoiceListener->onChoiceMade(player, action, card1, card2);
            mPlayer2ChoiceListener->onChoiceMade(player, action, action == Player::PLAYER_CHOICE_KEEP ? NULL : card1, card2);
            if (player->getTrend() != PlayerTrend::NONE) {
                mP2ActionCountTotal++;
                bool correct = false;
                AIPlayer* ai = (AIPlayer *)mPlayer2ChoiceListener;
                if (ai != NULL && ai->getGuessedUpProb() != 50) {
                    mP2GuessCountTotal++;
                    if (player->getTrend() == PlayerTrend::UP && ai->getGuessedUpProb() > 50) {
                        correct = true;
                        mP2GuessCountScore += ai->getGuessedUpProb();
                    }else if(player->getTrend() == PlayerTrend::DOWN && ai->getGuessedUpProb() < 50){
                        correct = true;
                        mP2GuessCountScore += 100 - ai->getGuessedUpProb();
                    }
                    
                    if (correct) {
                        mP2GuessCountCorrect++;
                    }
                    
                    LOGI("*** P2.guess=[%s] player.trend=[%s] correct=[%d]", ai->getGuessedUpProb() > 50? "UP" : "DOWN",
                         player->getTrend() == PlayerTrend::UP? "UP" : "DOWN", correct);
                }
            }
        }else if (player->getTag() == 2){
            mPlayer2ChoiceListener->onChoiceMade(player, action, card1, card2);
            mPlayerChoiceListener->onChoiceMade(player, action, action == Player::PLAYER_CHOICE_KEEP ? NULL : card1, card2);
        }
    }else if(action == ACTION_START_GAME_STATE + state::INIT){
        mPlayerChoiceListener->onChoiceMade(player, action, card1, card2);
        mPlayer2ChoiceListener->onChoiceMade(player, action, card1, card2);
    }
}

void Game::setGameStateListener(GameStateListener* l){
    mGameStateListener = l;
}

Card* Game::currentCard(){
    Card* card = NULL;
    if (mCurrentCardIndex >= 0 && mCurrentCardIndex < mCardList->size()) {
        card = mCardList->at(mCurrentCardIndex);
    }
    return card;
}


void Game::makeChoice(Player* player, Card* card, int availableChoice){
    
}


void Game::dumpPlayerGuessStat(){
    LOGF("--------- p2 guess=[%d/%d/%d] guess=[%5.2f%%] guess.correct=[%5.2f%%] score=[%d]", mP2GuessCountCorrect, mP2GuessCountTotal, mP2ActionCountTotal, (float)mP2GuessCountTotal / mP2ActionCountTotal * 100,(float)mP2GuessCountCorrect / mP2GuessCountTotal * 100,mP2GuessCountScore);
}



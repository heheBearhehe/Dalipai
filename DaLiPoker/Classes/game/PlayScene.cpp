//
//  PlayScene.cpp
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#include "PlayScene.h"
#include <iostream>
#include "GameLayer.h"
#include "PauseLayer.h"
#include "UserChoiceLayer.h"
#include "CalcScoreLayer.h"
#include "ReplayLayer.h"
#include "../model/def.h"
#include "AIPlayer.h"
#include "ReplayPlayer.h"
#include "Settings.h"

using namespace std;

USING_NS_CC;

static const int TAG_BTN_PAUSE   = 1000;


bool PlayScene::init(){
    if (!Scene::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::create("game_bg.jpg");
    bg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(bg, 0);
    
//    auto label = Label::create();
//    label->setString("大李牌");
//    label->setSystemFontSize(30);
//    label->setColor(Color3B::BLACK);
//    label->setPosition(Vec2(origin.x + visibleSize.width/2,
//                            origin.y + visibleSize.height - label->getContentSize().height));
//    this->addChild(label, 1);
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(PlayScene::menuRestart, this));
    
    closeItem->setPosition(Vec2(origin.x + closeItem->getContentSize().width/2 ,
                                origin.y + visibleSize.height - closeItem->getContentSize().height/2));
    
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    menu->setTag(TAG_BTN_PAUSE);
    this->addChild(menu, 1);
    
    mReplayerPlayer = NULL;
    mReplayInterval = 1;
    mGameLayer = GameLayer::create();
    mPauseLayer = PauseLayer::create();
    mUserChoiceLayer = UserChoiceLayer::create();
    mUserChoiceLayer->setPlayer(mPlayer1);
    mCalcScoreLayer = CalcScoreLayer::create();
    mReplayLayer = ReplayLayer::create();
    this->addChild(mGameLayer);
    this->addChild(mPauseLayer, 10000);
    this->addChild(mUserChoiceLayer);
    this->addChild(mCalcScoreLayer);
    this->addChild(mReplayLayer);
    
    mGameLayer->setVisible(true);
    mPauseLayer->setVisible(false);
    mUserChoiceLayer->setVisible(false);
    mReplayLayer->setVisible(false);
    
    startGame();
    
    return true;
}

void PlayScene::startGame(){
    mReplayMode = false;
    mReplayPaused = false;
    mGameLayer->setVisible(true);
    mCalcScoreLayer->setVisible(false);
    mReplayLayer->setVisible(false);
    
    getChildByTag(TAG_BTN_PAUSE)->setVisible(true);
    
    bool testReplay = false;
    
    delete mGame;
    delete mPlayer1;
    delete mPlayer2;
    delete mAi2;
    
    mReplayMode = testReplay;
    mGame = new Game(GAME_MODE::NORMAL, mReplayMode ? PLAY_MODE::REPLAY : PLAY_MODE::AUTO);
    
    mPlayer1 = new Player();
    mPlayer1->setTag(1);
    mPlayer2 = new Player();
    mPlayer2->setTag(2);
    mGame->setPlayer(mPlayer1, mPlayer2);
    
    if (mReplayMode) {
        Recorder* recorder = new Recorder();
        vector<Card *> *cardList = new vector<Card *>();
        for (int i = 0; i < 8; i++) {
            cardList->push_back(new Card(i));
        }
        for (int i = 0; i < 8; i++) {
            recorder->addPlayerAction((i % 2) + 1, 2);
        }
        recorder->setCardList(cardList);
        mGame->setRecorder(recorder);
        
        mReplayerPlayer = new ReplayPlayer(mGame);
        mReplayerPlayer->setRecorder(recorder);
        
        mGame->setPlayer1ChoiceListener(this);
        mGame->setPlayer2ChoiceListener(this);
        mGameLayer->setShouldShowOppnentCard(true);
    }else{
        
        mGame->setPlayer1ChoiceListener(this);
//        mGame->setPlayer1ChoiceListener(new AIPlayer(mGame));
        mAi2 = new AIPlayer(mGame);
        if (Settings::getInstance()->giveProb >= 0) {
            mAi2->setGiveProb(Settings::getInstance()->giveProb);
        }
        if (Settings::getInstance()->card1Weight >= 0) {
            mAi2->setKeepStrategyWeight((int[]){Settings::getInstance()->card1Weight, Settings::getInstance()->card2Weight, Settings::getInstance()->card3Weight});
        }
        
        mGame->setPlayer2ChoiceListener(mAi2);
        mGameLayer->setShouldShowOppnentCard(false);
    }
    mGame->setGameStateListener(this);
    mGameLayer->setGame(mGame);
    
    mGame->start();
    mGame->next();
}

void PlayScene::replayGame(){
    mReplayMode = true;
    mReplayPaused = false;
    this->getChildByTag(TAG_BTN_PAUSE)->setVisible(false);
    
    mGameLayer->setVisible(true);
    mCalcScoreLayer->setVisible(false);
    mReplayLayer->setVisible(true);
    mReplayLayer->show(mGame, this);
    
    mGame->setPlayMode(PLAY_MODE::REPLAY);
    mReplayerPlayer = new ReplayPlayer(mGame);
    mReplayerPlayer->setRecorder(mGame->getRecorder());
    
    mGame->setPlayer1ChoiceListener(this);
    mGame->setPlayer2ChoiceListener(this);
    mGameLayer->setShouldShowOppnentCard(true);
    mGame->setGameStateListener(this);
    mGameLayer->setGame(mGame);
    
    mGame->start();
    mGame->next();
    
}

cocos2d::ui::Button* PlayScene::addButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create();
    btn->setTag(tag);
    btn->setTitleText(text);
    
    btn->setTouchEnabled(true);
    btn->setTitleColor(Color3B::BLACK);
    btn->setTitleFontSize(40);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(PlayScene::touchEvent,this)));
    this->addChild(btn, 1);
    return btn;
}


void PlayScene::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}


int PlayScene::makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback){
    LOGI("UI. makeChoice  card=[%s]", card->getDisplay().c_str());
    if (!mReplayMode && player == mPlayer2) {
        return 0;
    }
    
    if (mReplayMode) {
        mReplayCallback = callback;
        mCurrentReplayPlayer = player;
        mCurrentReplayAction = mReplayerPlayer->makeChoice(player, card, availableChoice, callback);
        DelayTime * delayAction = DelayTime::create(mReplayInterval * 2);
        CallFunc * callFunc = CallFunc::create(CC_CALLBACK_0(PlayScene::onMakeChoice, this));
        this->runAction(CCSequence::createWithTwoActions(delayAction, callFunc));
    }else{
        mGameLayer->setDealCardForReplay(NULL);
        mGameLayer->invalidate();
        
        mUserChoiceLayer->setVisible(true);
        mUserChoiceLayer->show(card, player->getLastCard(), availableChoice, callback);
    }
    
    string message = getChoiceMessage(availableChoice, player);
    if (message.length() > 0) {
        mGameLayer->setMessage(message);
    }

    return 0;
}

void PlayScene::onMakeChoice(){
    LOGI("PlayScene.onMakeChoice");
    if (mReplayPaused) {
        return;
    }
    mReplayCallback->onPlayerAction(mCurrentReplayPlayer, mCurrentReplayAction);
    mReplayCallback->execute();
}

bool PlayScene::onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard){
    LOGI("UI. onChoiceMade  player=[%d] choice=[%d] current=[%s] last=[%s]",
         player->getTag(), choice, currentCard->getDisplay().c_str(), lastCard == NULL? "" : lastCard->getDisplay().c_str());
    return false;
}

void PlayScene::onFinished(){
    LOGI("UI. onFinished");
    mGameLayer->onFinished();
    mUserChoiceLayer->setVisible(false);
    mReplayLayer->setVisible(false);
    
    mGameLayer->setVisible(false);
    this->getChildByTag(TAG_BTN_PAUSE)->setVisible(false);
    mCalcScoreLayer->setVisible(true);
    mCalcScoreLayer->show(mGame, this);
}

void PlayScene::onGameAction(int action){
    switch (action) {
        case GAME_ACTION::GAME_ACTION_PAUSE:
            mPauseLayer->setVisible(true);
            mPauseLayer->show(mGame, this);
            break;
        case GAME_ACTION::GAME_ACTION_RESUME:
            mPauseLayer->setVisible(false);
            break;
        case GAME_ACTION::GAME_ACTION_RECALC_SCORE:
            onFinished();
            break;
        case GAME_ACTION::GAME_ACTION_RESTART:
            mPauseLayer->setVisible(false);
            mUserChoiceLayer->setVisible(false);
            startGame();
            break;
        case GAME_ACTION::GAME_ACTION_REPLAY:
            replayGame();
            break;
            
        case GAME_ACTION::GAME_ACTION_REPLAY_PAUSE:
            if (mReplayPaused) {
                mReplayPaused = false;
                onMakeChoice();
            }else{
                mReplayPaused = true;
            }
            break;
        case GAME_ACTION::GAME_ACTION_REPLAY_RESUME:
            mReplayPaused = true;
            onMakeChoice();
            break;
        case GAME_ACTION::GAME_ACTION_REPLAY_EXIT:
            mReplayPaused = true;
            onFinished();
            break;
        case GAME_ACTION::GAME_ACTION_REPLAY_FAST:
            if (mReplayInterval >= 0.2) {
                mReplayInterval /= 2;
            }
            break;
        case GAME_ACTION::GAME_ACTION_REPLAY_SLOW:
            if (mReplayInterval <= 5) {
                mReplayInterval *= 2;
            }
            break;
            
        case GAME_ACTION::GAME_ACTION_EXIT:
            Director::getInstance()->popScene();
            break;
            
        default:
            break;
    }
}

void PlayScene::menuRestart(Ref* pSender){
    onGameAction(GAME_ACTION::GAME_ACTION_PAUSE);
}


void PlayScene::onActionExecuted(int action, Player* player, Card* card1, Card* card2){
    LOGI("PlayScene.onActionExecuted  action=[%x] player=[%d] c1=[%s] c2=[%s]", action, player->getTag(), card1 == NULL? "" : card1->getDisplay().c_str(), card2 == NULL? "" : card2->getDisplay().c_str());
    
    float delayTime;
    string message = getActionExecutedMessage(action, player);
    if (message.length() > 0) {
        delayTime = 0.5f;
        mGameLayer->setMessage(message);
    }else{
        delayTime = 0.2f;
        mGameLayer->clearMessage();
    }
    mGameLayer->setDealCardForReplay(NULL);
    if (mReplayMode) {
        delayTime = mReplayInterval;
        if (action == ACTION_START_GAME_STATE + STATE::PLAYER_DEAL) {
            mGameLayer->setDealCardForReplay(card1);
        }
    }
    
    mGameLayer->invalidate();
    
    DelayTime * delayAction = DelayTime::create(delayTime);
//    CC_CALLBACK_0(PlayScene::onAction, this);
    CallFunc * callFunc = CallFunc::create(CC_CALLBACK_0(PlayScene::onAction, this));
    this->runAction(CCSequence::createWithTwoActions(delayAction, callFunc));
}
                                           
void PlayScene::onAction(){
    LOGI("PlayScene.onAction");
    mGame->next();
}

std::string PlayScene::getChoiceMessage(int action, Player* player){
    string message = "";
    if (player == NULL) {
        return message;
    }
    
    if (player->getTag() == 2) {
        switch (action) {
            case Player::PLAYER_CHOICE_KEEP:
                message = "对方摸牌";
                break;
            case Player::PLAYER_CHOICE_DISCARD:
                message = "对方弃牌";
                break;
            case Player::PLAYER_CHOICE_GIVE:
                message = "对方给你牌";
                break;
            case Player::PLAYER_CHOICE_KEEP_FOR_GIVE:
                message = "对方留下了您给的牌";
                break;
            case Player::PLAYER_CHOICE_REMOVE_FOR_GIVE:
                message = "对方弃了2张牌";
                break;
                
            default:
                break;
        }
    }else if (player->getTag() == 1) {
        if ((action & (action -1)) != 0) {
            message = "你摸了一张牌";
        }else{
            switch (action) {
                case Player::PLAYER_CHOICE_KEEP_FOR_GIVE:
                    message = "你必须留下对方给你的牌";
                    break;
                case Player::PLAYER_CHOICE_REMOVE_FOR_GIVE:
                    message = "你必须弃掉对方给你的牌，并再弃一张手牌";
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    return message;
}


std::string PlayScene::getActionExecutedMessage(int action, Player* player){
    string message = "";
    if (player == NULL) {
        return message;
    }
    
    if (player->getTag() == 2) {
        switch (action) {
            case Player::PLAYER_CHOICE_KEEP:
                message = "对方留牌";//
                break;
            case Player::PLAYER_CHOICE_DISCARD:
                message = "对方弃牌";
                break;
            case Player::PLAYER_CHOICE_GIVE:
                message = "对方给你牌";//
                break;
            case Player::PLAYER_CHOICE_KEEP_FOR_GIVE:
                message = "对方留下了您给的牌";
                break;
            case Player::PLAYER_CHOICE_REMOVE_FOR_GIVE:
                message = "对方弃了2张牌";
                break;
                
            default:
                break;
        }
    }else if (player->getTag() == 1) {
        switch (action) {
            case Player::PLAYER_CHOICE_KEEP:
                break;
            case Player::PLAYER_CHOICE_DISCARD:
                message = "弃牌";
                break;
            case Player::PLAYER_CHOICE_GIVE:
                message = "给对方牌";
                break;
            case Player::PLAYER_CHOICE_KEEP_FOR_GIVE:
                message = "你留下了对方给的牌";
                break;
            case Player::PLAYER_CHOICE_REMOVE_FOR_GIVE:
                message = "你弃了2张牌";
                break;
                
            default:
                break;
        }
    }
    return message;
}



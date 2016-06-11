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

static const int TAG_BTN_PAUSE    = 1000;
static const int TAG_PAUSE_BG     = 1001;


bool PlayScene::init(){
    if (!Scene::init()){
        return false;
    }
    
    initBG();
    
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
    mUserChoiceLayer->setPause(false);
    mUserChoiceLayer->setVisible(false);
    mReplayLayer->setVisible(false);
    
    startGame();
    
    return true;
}



void PlayScene::initBG(){
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::create("game_bg.jpg");
    bg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(bg, 0);

    auto bgLeft = Sprite::create("bg_left.png");
    auto bgRight = Sprite::create("bg_left.png");
    auto bgBottom = Sprite::create("bg_bottom.png");
    auto bgTop = Sprite::create("bg_bottom.png");
    auto bgLeftBottom = Sprite::create("bg_bottom_left.png");
    auto bgRightTop = Sprite::create("bg_top_right.png");
    
    float leftWidth = 16;
    float bottomHeight = 88;
    bgLeft->setContentSize(Size(leftWidth, visibleSize.height - bottomHeight * 2));
    bgLeft->setPosition(Vec2(origin.x + leftWidth / 2, origin.y + visibleSize.height / 2));
    
    bgRight->setContentSize(Size(leftWidth, visibleSize.height - bottomHeight * 2));
    bgRight->setPosition(Vec2(origin.x + visibleSize.width - leftWidth / 2, origin.y + visibleSize.height / 2));
    
    bgTop->setContentSize(Size(visibleSize.width, bottomHeight));
    bgTop->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - bottomHeight / 2));
    
    bgBottom->setContentSize(Size(visibleSize.width, bottomHeight));
    bgBottom->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + bottomHeight / 2));
    
    bgLeftBottom->setPosition(Vec2(origin.x + bgLeftBottom->getContentSize().width / 2, origin.y + bgLeftBottom->getContentSize().height / 2));
    
    bgRightTop->setPosition(Vec2(origin.x + visibleSize.width - bgRightTop->getContentSize().width / 2, origin.y + visibleSize.height - bgRightTop->getContentSize().height / 2));
    
    this->addChild(bgLeft);
    this->addChild(bgRight);
    this->addChild(bgTop);
    this->addChild(bgBottom);
    this->addChild(bgLeftBottom);
    this->addChild(bgRightTop);
    
    
    auto menuBg = cocos2d::ui::ImageView::create("menu_bg.png");
    menuBg->setPosition(Vec2(origin.x + menuBg->getContentSize().width / 2, origin.y + visibleSize.height - menuBg->getContentSize().height / 2 + 500));
    menuBg->setTag(TAG_PAUSE_BG);
    this->addChild(menuBg);
    
    
    auto closeItem = MenuItemImage::create(
                                           "menu_open.png",
                                           "menu_open.png",
                                           CC_CALLBACK_1(PlayScene::menuRestart, this));
    
    closeItem->setPosition(Vec2(origin.x + menuBg->getContentSize().width/2 ,
                                origin.y + visibleSize.height - closeItem->getContentSize().height/2));
    
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    menu->setTag(TAG_BTN_PAUSE);
    this->addChild(menu, 1);
    
}

void PlayScene::startGame(){
    mReplayMode = false;
    mReplayPaused = false;
    mChoiceSkipped = false;
    mReplayStartCardIndex = -1;
    mGameLayer->setVisible(true);
    mCalcScoreLayer->setVisible(false);
    mReplayLayer->setVisible(false);
    
    getChildByTag(TAG_BTN_PAUSE)->setVisible(true);
    getChildByTag(TAG_PAUSE_BG)->setVisible(true);
    
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
        mAi2 = new AIPlayer(mGame->getMinRank(), mGame->getMaxRank());
        mAi2->setTag(2);
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
    replayGame(0);
}

void PlayScene::replayGame(int startCardIndex){
    LOGI("*******  PlayScene::replayGame  startCardIndex=[%d]", startCardIndex);
    mReplayMode = true;
    mReplayPaused = false;
    mChoiceSkipped = false;
    mReplayStartCardIndex = startCardIndex;
    this->getChildByTag(TAG_BTN_PAUSE)->setVisible(false);
    this->getChildByTag(TAG_PAUSE_BG)->setVisible(false);
    
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
    
    if (startCardIndex <= 0) {
        mGame->next();
    }
}

void PlayScene::finishReplay(){
    mReplayMode = true;
    mReplayPaused = false;
    mChoiceSkipped = false;
    mReplayStartCardIndex = mGame->getRecorder()->getCardIndexList()->size();
    
    mGame->setPlayMode(PLAY_MODE::REPLAY);
    mReplayerPlayer = new ReplayPlayer(mGame);
    mReplayerPlayer->setRecorder(mGame->getRecorder());
    mGame->setPlayer1ChoiceListener(mReplayerPlayer);
    mGame->setPlayer2ChoiceListener(mReplayerPlayer);
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
    LOGI("*******  UI. makeChoice  card=[%s] replay=[%d]", card->getDisplay().c_str(), mReplayMode);
    if (!mReplayMode && player == mPlayer2) {
        return 0;
    }
    
    if (mReplayMode) {
        mReplayCallback = callback;
        mCurrentReplayPlayer = player;
        mCurrentReplayAction = mReplayerPlayer->makeChoice(player, card, availableChoice, callback);
        int currentCardIndex = mGame->getCurrentCardIndex();
        LOGI("*******  UI. makeChoice.2   currentCardIndex=[%d]  mReplayStartCardIndex=[%d]", currentCardIndex, mReplayStartCardIndex);
        if (currentCardIndex < mReplayStartCardIndex) {
            onMakeChoice();
        }else{
            DelayTime * delayAction = DelayTime::create(mReplayInterval * 2);
            CallFunc * callFunc = CallFunc::create(CC_CALLBACK_0(PlayScene::onMakeChoice, this));
            this->runAction(CCSequence::createWithTwoActions(delayAction, callFunc));
        }
        
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
    if (mReplayPaused) {
        LOGI("*******  PlayScene.onMakeChoice.SKIP");
        mChoiceSkipped = true;
        return;
    }
    LOGI("*******  PlayScene.onMakeChoice");
    mChoiceSkipped = false;
    mReplayCallback->onPlayerAction(mCurrentReplayPlayer, mCurrentReplayAction);
    mReplayCallback->execute();
}

bool PlayScene::onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard){
    LOGI("UI. onChoiceMade  player=[%d] choice=[%d] current=[%s] last=[%s]",
         player->getTag(), choice, currentCard == NULL? "" : currentCard->getDisplay().c_str(), lastCard == NULL? "" : lastCard->getDisplay().c_str());
    return false;
}

void PlayScene::onFinished(){
    LOGI("UI. onFinished");
    mGameLayer->onFinished();
    mUserChoiceLayer->setVisible(false);
    mReplayLayer->setVisible(false);
    
    mGameLayer->setVisible(false);
    this->getChildByTag(TAG_BTN_PAUSE)->setVisible(false);
    this->getChildByTag(TAG_PAUSE_BG)->setVisible(false);
    mCalcScoreLayer->setVisible(true);
    mCalcScoreLayer->show(mGame, this);
}

void PlayScene::onGameAction(int action){
    switch (action) {
        case GAME_ACTION::GAME_ACTION_PAUSE:
            mPauseLayer->setVisible(true);
            mPauseLayer->show(mGame, this);
            mUserChoiceLayer->setPause(true);
            break;
        case GAME_ACTION::GAME_ACTION_RESUME:
            mPauseLayer->setVisible(false);
            mUserChoiceLayer->setPause(false);
            break;
        case GAME_ACTION::GAME_ACTION_RECALC_SCORE:
            onFinished();
            break;
        case GAME_ACTION::GAME_ACTION_RESTART:
            mPauseLayer->setVisible(false);
            mUserChoiceLayer->setPause(false);
            mUserChoiceLayer->setVisible(false);
            startGame();
            break;
        case GAME_ACTION::GAME_ACTION_REPLAY:
            replayGame();
            break;
            
        case GAME_ACTION::GAME_ACTION_REPLAY_PAUSE:
            mReplayPaused = true;
            break;
        case GAME_ACTION::GAME_ACTION_REPLAY_RESUME:
            mReplayPaused = false;
            onMakeChoice();
            break;
        case GAME_ACTION::GAME_ACTION_REPLAY_EXIT:
            finishReplay();
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
        case GAME_ACTION::GAME_ACTION_REPLAY_NEXT:
            if (mReplayMode && mReplayPaused) {
                mReplayPaused = false;
                onMakeChoice();
                mReplayPaused = true;
            }
            break;
        case GAME_ACTION::GAME_ACTION_REPLAY_PREV:
            if (mGame->getCurrentCardIndex() > 0) {
                mReplayPaused = false;
                replayGame(mGame->getCurrentCardIndex() - 1);
                if (mChoiceSkipped) {
                    onMakeChoice();
                }
                mReplayPaused = true;
                mReplayLayer->updateReplay(true);
                mGameLayer->invalidate();
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
    LOGI("*****  PlayScene.onActionExecuted  action=[%x] player=[%d] c1=[%s] c2=[%s]", action, player->getTag(), card1 == NULL? "" : card1->getDisplay().c_str(), card2 == NULL? "" : card2->getDisplay().c_str());
    
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
    
    LOGI("*******  PlayScene.onActionExecuted.2   mReplayStartCardIndex=[%d]  mGame.index=[%d]", mReplayStartCardIndex, mGame->getCurrentCardIndex());
    if(mReplayStartCardIndex > mGame->getCurrentCardIndex()){
        onAction();
    }else{
        mGameLayer->invalidate();
        if (!mReplayMode
            && player->getTag() == 2
            && (action == Player::PLAYER_CHOICE_DISCARD || action == Player::PLAYER_CHOICE_REMOVE_FOR_GIVE)) {
            mUserChoiceLayer->showOppenentCard(card1, card2);
            mUserChoiceLayer->setVisible(true);
            DelayTime * delayAction = DelayTime::create(2.0);
            CallFunc * callFunc = CallFunc::create(CC_CALLBACK_0(PlayScene::hideOppenentCardAndOnAction, this));
            this->runAction(CCSequence::createWithTwoActions(delayAction, callFunc));
        }else{
            DelayTime * delayAction = DelayTime::create(delayTime);
            CallFunc * callFunc = CallFunc::create(CC_CALLBACK_0(PlayScene::onAction, this));
            this->runAction(CCSequence::createWithTwoActions(delayAction, callFunc));
        }
    }
}

void PlayScene::hideOppenentCardAndOnAction(){
    mUserChoiceLayer->hideOppenentCard();
    mGameLayer->clearMessage();
    DelayTime * delayAction = DelayTime::create(1.0);
    CallFunc * callFunc = CallFunc::create(CC_CALLBACK_0(PlayScene::onAction, this));
    this->runAction(CCSequence::createWithTwoActions(delayAction, callFunc));
}

void PlayScene::onAction(){
    LOGI("*****  PlayScene.onAction.DO");
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



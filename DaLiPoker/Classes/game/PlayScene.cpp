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
#include "../model/def.h"
#include "AIPlayer.h"

using namespace std;

USING_NS_CC;

bool PlayScene::init(){
    if (!Scene::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::create("game_bg.jpg");
    bg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(bg, 0);
    
    auto label = Label::create();
    label->setString("大李牌");
    label->setSystemFontSize(30);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(PlayScene::menuRestart, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    mGameLayer = GameLayer::create();
    mPauseLayer = PauseLayer::create();
    mUserChoiceLayer = UserChoiceLayer::create();
    mUserChoiceLayer->setPlayer(mPlayer1);
    this->addChild(mGameLayer);
    this->addChild(mPauseLayer);
    this->addChild(mUserChoiceLayer);
    mGameLayer->setVisible(true);
    mPauseLayer->setVisible(false);
    mUserChoiceLayer->setVisible(false);
    
    startGame();
    
    return true;
}

void PlayScene::startGame(){
    delete mGame;
    delete mPlayer1;
    delete mPlayer2;
    
    mGame = new Game(GAME_MODE::SMALL);
    mPlayer1 = new Player();
    mPlayer1->setTag(1);
    mPlayer2 = new Player();
    mPlayer2->setTag(2);
    mGame->setPlayer(mPlayer1, mPlayer2);
    mGame->setPlayer1ChoiceListener(this);
//    mGame->setPlayer1ChoiceListener(new AIPlayer(mGame));
    mGame->setGameStateListener(this);
    mGameLayer->setShouldShowOppnentCard(false);
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
    if (player == mPlayer2) {
        return 0;
    }
    
    mGameLayer->invalidate();
    
    mUserChoiceLayer->setVisible(true);
    mUserChoiceLayer->show(card, player->getLastCard(), availableChoice, callback);
    
    string message = getChoiceMessage(availableChoice, player);
    if (message.length() > 0) {
        mGameLayer->setMessage(message);
    }

    return 0;
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
}

void PlayScene::menuRestart(Ref* pSender){
    startGame();
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
    mGameLayer->invalidate();
    
    DelayTime * delayAction = DelayTime::create(delayTime);
    CC_CALLBACK_0(PlayScene::onAction, this);
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



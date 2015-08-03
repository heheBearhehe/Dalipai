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

using namespace std;

USING_NS_CC;

bool PlayScene::init(){
    if (!Scene::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
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
    mGame->init();
    mGame->setPlayer(mPlayer1, mPlayer2);
    mGame->setPlayer1ChoiceListener(this);
    mGameLayer->setGame(mGame);
    
    mGame->start();
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


int PlayScene::makeChoice(Card* card, int availableChoice, PlayerActionCallBack* callback){
    LOGI("UI. makeChoice  card=[%s]", card->getDisplay().c_str());
    mGameLayer->invalidate();
    
    mUserChoiceLayer->setVisible(true);
    mUserChoiceLayer->show(card, availableChoice, callback);
    return 0;
}

void PlayScene::onFinished(){
    LOGI("UI. onFinished");
    mGameLayer->onFinished();
}

void PlayScene::menuRestart(Ref* pSender){
    startGame();
}




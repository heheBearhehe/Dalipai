//
//  PauseLayer.cpp
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#include "PauseLayer.h"
#include "../model/def.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

LayerColor * LayerColor::create(const cocos2d::Color4B& color){
    return cocos2d::LayerColor::create(color);
}

bool PauseLayer::init(){
    if (!LayerColor::initWithColor(cocos2d::Color4B(0,0,0,128))){
        return false;
    }
    
    invalidate();
    
    return true;
}

void PauseLayer::show(Game* game, GameActionCallBack* callback){
    if (game == NULL) {
        return;
    }
    
    mGame = game;
    mGameActionCallBack = callback;
    invalidate();
}

void PauseLayer::invalidate(){
    this->removeAllChildren();
    if (mGame == NULL) {
        return;
    }
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    float buttonWidth = 200;
    float buttonHeight = 50;
    float posY = origin.y + visibleSize.height / 2;
    
    
    addButton("回到游戏", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2, posY + 100), GAME_ACTION::GAME_ACTION_RESUME);
    addButton("重新开始", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2, posY), GAME_ACTION::GAME_ACTION_RESTART);
    addButton("退出游戏", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2, posY - 100), GAME_ACTION::GAME_ACTION_EXIT);
}

void PauseLayer::drawText(const string& text, const Vec2& position, const Size & size){
    auto label = Label::create();
    label->setContentSize(size);
    label->setString(text);
    label->setSystemFontSize(30);
    label->setColor(Color3B::BLACK);
    label->setPosition(position);
    this->addChild(label, 1);
}


cocos2d::ui::Button* PauseLayer::addButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create("btn_choice.png", "btn_choice_h.png", "", TextureResType::LOCAL);
    //    btn->setUnifySizeEnabled(false);
    btn->setTag(tag);
    btn->setTitleText(text);
    
    btn->setTouchEnabled(true);
    btn->setTitleColor(Color3B::BLACK);
    btn->setTitleFontSize(40);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(PauseLayer::touchEvent,this)));
    this->addChild(btn, 1);
    return btn;
}

void PauseLayer::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
    cocos2d::ui::Button* btn = (cocos2d::ui::Button*)ref;
    LOGI("UI. touchEvent  tag=[%d]", btn->getTag());
    
    if (btn->getTag() <= 0) {
        return;
    }
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            if (mGameActionCallBack != NULL) {
                mGameActionCallBack->onGameAction(btn->getTag());
            }
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
//
//  ReplayLayer.cpp
//  DaLiPoker
//
//  Created by lugan on 10/18/15.
//
//

#include "ReplayLayer.h"
#include "../model/def.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;


bool ReplayLayer::init(){
    if (!Layer::init()){
        return false;
    }
    
    invalidate();
    
    return true;
}

void ReplayLayer::show(Game* game, GameActionCallBack* callback){
    if (game == NULL) {
        return;
    }
    
    mGame = game;
    mGameActionCallBack = callback;
    invalidate();
}

void ReplayLayer::invalidate(){
    this->removeAllChildren();
    if (mGame == NULL) {
        return;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float buttonWidth = 130;
    float buttonHeight = 50;
    float posX = origin.x + 20 + buttonWidth / 2;
    float posY = origin.y + visibleSize.height - buttonHeight / 2 - 20;
    
    addButton("退出", Size(buttonWidth,buttonHeight), Vec2(posX, posY), GAME_ACTION::GAME_ACTION_REPLAY_EXIT);
    addButton("<<", Size(buttonWidth,buttonHeight), Vec2(posX + (buttonWidth + 20), posY), GAME_ACTION::GAME_ACTION_REPLAY_SLOW);
    addButton("||", Size(buttonWidth,buttonHeight), Vec2(posX + 2 * (buttonWidth + 20), posY), GAME_ACTION::GAME_ACTION_REPLAY_PAUSE);
    addButton(">>", Size(buttonWidth,buttonHeight), Vec2(posX + 3 * (buttonWidth + 20), posY), GAME_ACTION::GAME_ACTION_REPLAY_FAST);
}

void ReplayLayer::drawText(const string& text, const Vec2& position, const Size & size){
    auto label = Label::create();
    label->setContentSize(size);
    label->setString(text);
    label->setSystemFontSize(30);
    label->setColor(Color3B::BLACK);
    label->setPosition(position);
    this->addChild(label, 1);
}


cocos2d::ui::Button* ReplayLayer::addButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create("btn_choice.png", "btn_choice_h.png", "", TextureResType::LOCAL);
    //    btn->setUnifySizeEnabled(false);
    btn->setTag(tag);
    btn->setTitleText(text);
    btn->setScale9Enabled(true);
    btn->setTouchEnabled(true);
    btn->setTitleColor(Color3B::BLACK);
    btn->setTitleFontSize(40);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(ReplayLayer::touchEvent,this)));
    this->addChild(btn, 1);
    return btn;
}

void ReplayLayer::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
    cocos2d::ui::Button* btn = (cocos2d::ui::Button*)ref;
    LOGI("UI. touchEvent  tag=[%d]", btn->getTag());
    
    if (btn->getTag() <= 0) {
        return;
    }
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            if (btn->getTag() == GAME_ACTION_REPLAY_PAUSE) {
                btn->setTag(GAME_ACTION_REPLAY_RESUME);
                btn->setTitleText(">");
            }else if(btn->getTag() == GAME_ACTION_REPLAY_RESUME){
                btn->setTag(GAME_ACTION_REPLAY_PAUSE);
                btn->setTitleText("||");
            }
            
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

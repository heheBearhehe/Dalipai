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

void ReplayLayer::show(Game* game){
    if (game == NULL) {
        return;
    }
    
    mGame = game;
    invalidate();
}

void ReplayLayer::invalidate(){
    this->removeAllChildren();
    if (mGame == NULL) {
        return;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
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

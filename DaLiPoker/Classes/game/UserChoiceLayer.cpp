//
//  UserChoiceLayer.cpp
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#include "UserChoiceLayer.h"
#include "../model/def.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;


USING_NS_CC;


bool UserChoiceLayer::init(){
    if (!Layer::init()){
        return false;
    }
    
//    test();
    
    return true;
}

void UserChoiceLayer::test(){
    this->removeAllChildren();
    
    show(new Card(2, 3), 7, NULL);
}

void UserChoiceLayer::show(Card* card, int options, PlayerActionCallBack* callback){
    removeAllChildren();
    
    mPlayerActionCallBack = callback;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float posY = origin.y + visibleSize.height / 2 - 200;
    addButton(card->getDisplay(), Size(100,150), Vec2(origin.x + visibleSize.width / 2, posY), 0);
    
    posY -= 100;
    
    if(options == Player::PLAYER_CHOICE_KEEP_FOR_GIVE || options == Player::PLAYER_CHOICE_REMOVE_FOR_GIVE){
        addButton("确定", Size(80,50), Vec2(origin.x + visibleSize.width / 2, posY), options);
    }else{
        if ((options & Player::PLAYER_CHOICE_GIVE) > 0) {
            addButton("给", Size(80,50), Vec2(origin.x + visibleSize.width / 2 - 150, posY), Player::PLAYER_CHOICE_GIVE);
        }
        if ((options & Player::PLAYER_CHOICE_KEEP) > 0) {
            addButton("收", Size(80,50), Vec2(origin.x + visibleSize.width / 2, posY), Player::PLAYER_CHOICE_KEEP);
        }
        if ((options & Player::PLAYER_CHOICE_DISCARD) > 0) {
            addButton("弃", Size(80,50), Vec2(origin.x + visibleSize.width / 2 + 150, posY), Player::PLAYER_CHOICE_DISCARD);
        }
    }
}



cocos2d::ui::Button* UserChoiceLayer::addButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create();
    btn->setTag(tag);
    btn->setTitleText(text);
    
    btn->setTouchEnabled(true);
    btn->setTitleColor(Color3B::BLACK);
    btn->setTitleFontSize(40);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(UserChoiceLayer::touchEvent,this)));
    this->addChild(btn, 1);
    return btn;
}


void UserChoiceLayer::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
    cocos2d::ui::Button* btn = (cocos2d::ui::Button*)ref;
    LOGI("UI. touchEvent  tag=[%d]", btn->getTag());
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            if (mPlayerActionCallBack != NULL) {
                mPlayerActionCallBack->onPlayerAction(mPlayer, btn->getTag());
//                this->setVisible(false);
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

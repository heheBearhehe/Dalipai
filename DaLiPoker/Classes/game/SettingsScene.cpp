//
//  SettingsScene.cpp
//  DaLiPoker
//
//  Created by lugan on 10/17/15.
//
//

#include "SettingsScene.h"
#include "Utils.h"
#include "Settings.h"

using namespace std;

USING_NS_CC;

static const int TAG_TEXT_START   = 1000;
static const int TAG_BUTTON_START = 2000;

bool SettingsScene::init(){
    if (!Scene::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::create("game_bg.jpg");
    bg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(bg, 0);
    
    float stepY = MIN(200, visibleSize.height / 5);
    float posY = origin.y + visibleSize.height * 4 / 5;

    
    return true;
}

void SettingsScene::drawText(const string& text, const Size & size, const Vec2& position, int tag){
    auto label = Label::create();
    label->setContentSize(size);
    label->setString(text);
    label->setSystemFontSize(30);
    label->setColor(R::COLOR_TEXT);
    label->setPosition(position);
    label->setTag(tag);
    this->addChild(label, 1);
}

cocos2d::ui::Button* SettingsScene::addButton(const std::string& text, const Size & size, const Vec2& position, int tag, bool enable){
    std::string picNormal = enable? "btn_choice.png" : "btn_choice_d.png";
    auto btn = cocos2d::ui::Button::create(picNormal, "btn_choice_h.png", "", cocos2d::ui::TextureResType::LOCAL);
    
    btn->setTag(enable? tag : 0);
    btn->setTitleText(text);
    btn->setEnabled(enable);
    btn->setTouchEnabled(true);
    btn->setTitleColor(R::COLOR_TEXT_CHOICE);
    btn->setTitleFontSize(40);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(SettingsScene::touchEvent,this)));
    this->addChild(btn, 1);
    return btn;
}

void SettingsScene::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
    cocos2d::ui::Button* btn = (cocos2d::ui::Button*)ref;
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            if (btn->getTag() == TAG_BUTTON_START) {
                Director::getInstance()->popScene();
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



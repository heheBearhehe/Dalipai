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

    updateSliderText(this->addSlider("card1", Size(visibleSize.width / 2, 30), Vec2(300, posY), 0, Settings::getInstance()->card1Weight));
    updateSliderText(this->addSlider("card2", Size(visibleSize.width / 2, 30), Vec2(300, posY - stepY), 1, Settings::getInstance()->card2Weight));
    updateSliderText(this->addSlider("card3", Size(visibleSize.width / 2, 30), Vec2(300, posY - 2 * stepY), 2, Settings::getInstance()->card3Weight));
    updateSliderText(this->addSlider("give",  Size(visibleSize.width / 2, 30), Vec2(300, posY - 3 * stepY), 3, Settings::getInstance()->giveProb));

    addButton("返回", Size(visibleSize.width / 2, 50), Vec2(visibleSize.width / 2, MIN(origin.y + 100, posY - 4 * stepY)), TAG_BUTTON_START);
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

cocos2d::ui::Button* SettingsScene::addButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create();
    btn->setTag(tag);
    btn->setTitleText(text);
    
    btn->setTouchEnabled(true);
    btn->setTitleColor(Color3B::BLACK);
    btn->setTitleFontSize(40);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(SettingsScene::touchEvent,this)));
    this->addChild(btn, 1);
    return btn;
}



cocos2d::ui::Slider* SettingsScene::addSlider(const std::string& text, const Size & size, const Vec2& position, int tag, int value){
    this->drawText(text, Size(100, 50), Vec2(position.x - size.width / 2 - 100, position.y), 0);
    this->drawText("", Size(20, 50), Vec2(position.x + size.width / 2 + 20, position.y), TAG_TEXT_START + tag);
    
    auto slider = cocos2d::ui::Slider::create();
    slider->setSize(size);
    slider->setPosition(position);
    slider->setScale9Enabled(true);
    slider->loadBarTexture("btn_choice.png");
    slider->loadSlidBallTextures("CloseNormal.png");
    slider->loadProgressBarTexture("btn_choice_h.png");
    slider->addEventListener(CC_CALLBACK_2(SettingsScene::sliderEvent, this));
    slider->setTouchEnabled(true);
    if (value >= 0) {
        slider->setPercent(value);
    }else{
        slider->setPercent(50);
    }
    slider->setTag(tag);
    this->addChild(slider);
    
    return slider;
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

void SettingsScene::sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type)
{
    if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        cocos2d::ui::Slider* slider = dynamic_cast<cocos2d::ui::Slider*>(pSender);
        updateSliderText(slider);
        int percent = slider->getPercent();
        switch (slider->getTag()) {
            case 0:
                Settings::getInstance()->card1Weight = percent;
                break;
            case 1:
                Settings::getInstance()->card2Weight = percent;
                break;
            case 2:
                Settings::getInstance()->card3Weight = percent;
                break;
            case 3:
                Settings::getInstance()->giveProb = percent;
                break;
                
            default:
                break;
        }
        
        LOGI("- percent=[%d] tag=[%d]", percent, slider->getTag());
    }
}


void SettingsScene::updateSliderText(cocos2d::ui::Slider* slider){
    cocos2d::Label* label  = (cocos2d::Label *)this->getChildByTag(TAG_TEXT_START + slider->getTag());
    if (label != NULL) {
        stringstream ss;
        ss << slider->getPercent();
        label->setString(ss.str());
    }
}


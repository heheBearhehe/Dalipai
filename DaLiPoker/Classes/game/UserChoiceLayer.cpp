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
    createPokerFront(card);
    
    posY -= 100;
    
    float buttonWidth = 80;
    float buttonHeight = 50;
    if(options == Player::PLAYER_CHOICE_KEEP_FOR_GIVE || options == Player::PLAYER_CHOICE_REMOVE_FOR_GIVE){
        auto btn = addButton("确定", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2, posY), options);
        btn->setPosition(Vec2(origin.x + visibleSize.width / 2 - btn->getContentSize().width / 4, posY));
    }else{
        if ((options & Player::PLAYER_CHOICE_GIVE) > 0) {
            auto btn = addButton("给", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2 - 150, posY), Player::PLAYER_CHOICE_GIVE);
            btn->setPosition(Vec2(origin.x + visibleSize.width / 2 - btn->getContentSize().width * 5 / 4, posY));
        }
        if ((options & Player::PLAYER_CHOICE_KEEP) > 0) {
            auto btn = addButton("收", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2, posY), Player::PLAYER_CHOICE_KEEP);
            btn->setPosition(Vec2(origin.x + visibleSize.width / 2 - btn->getContentSize().width / 4, posY));
        }
        if ((options & Player::PLAYER_CHOICE_DISCARD) > 0) {
            auto btn = addButton("弃", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2 + 150, posY), Player::PLAYER_CHOICE_DISCARD);
            btn->setPosition(Vec2(origin.x + visibleSize.width / 2 + btn->getContentSize().width * 3 / 4, posY));
        }
    }
}



cocos2d::ui::Button* UserChoiceLayer::addButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create("btn_choice.png", "btn_choice_h.png", "btn_choice_d.png", TextureResType::LOCAL);
//    btn->setUnifySizeEnabled(false);
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


cocos2d::ui::Widget* UserChoiceLayer::createPokerFront(Card* card){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float posY = origin.y + visibleSize.height / 2 - 200;
    auto btnCard = addButton(card->getDisplay(), Size(100,150), Vec2(origin.x + visibleSize.width / 2, posY), 0);
    btnCard->loadTextures("poker_front_large.png", "", "", TextureResType::LOCAL);
    btnCard->setPosition(Vec2(origin.x + visibleSize.width / 2, posY + btnCard->getContentSize().height / 2));
    
    Size contentSize = btnCard->getContentSize();
    auto img = cocos2d::ui::ImageView::create("反正也是挣.jpg");
    img->ignoreContentAdaptWithSize(false);
    img->setContentSize(Size(contentSize.width - 100, contentSize.height - 100));
    img->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2));
    btnCard->addChild(img);
    
    
    contentSize = btnCard->getContentSize();
    float numTextSize = 40;
    float suitTextSize = 30;
    auto cardDisplay = createPokerDisplay(card, numTextSize, suitTextSize);
    cardDisplay->setPosition(Vec2(50, contentSize.height - 100));
    btnCard->addChild(cardDisplay);
    
    auto cardDisplayReverse = cardDisplay->clone();
    cardDisplayReverse->setRotation(180);
    cardDisplayReverse->setPosition(Vec2(contentSize.width - 50, 100));
    btnCard->addChild(cardDisplayReverse);
    
    return btnCard;
}

cocos2d::ui::Widget* UserChoiceLayer::createPokerDisplay(Card* card, float numTextSize, float suitTextSize){
    auto widget = Widget::create();
    
    auto btnCardText = cocos2d::ui::Button::create();
    btnCardText->setTitleText(card->getDisplayNum());
    btnCardText->setTitleColor(Color3B::BLACK);
    btnCardText->setTitleFontSize(numTextSize);
    btnCardText->setPosition(Vec2(0, numTextSize));
    widget->addChild(btnCardText);
    
    
    auto btnCardSuit = cocos2d::ui::Button::create();
    btnCardSuit->setTitleText(card->getDisplaySuit());
    btnCardSuit->setTitleColor(Color3B::BLACK);
    btnCardSuit->setTitleFontSize(suitTextSize);
    btnCardSuit->setPosition(Vec2(0, 0));
    widget->addChild(btnCardSuit);
    
    return widget;
}

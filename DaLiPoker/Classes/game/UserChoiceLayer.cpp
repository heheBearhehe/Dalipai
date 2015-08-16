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

static string sCardImagesFileName[] = {
    "No作No待.jpg",
    "上课代表.jpg",
    "乌鸦喝水.jpg",
    "他叫金十亿.jpg",
//    "偶遇.jpg",
//    "军训生存指南.jpg",
//    "友情与爱情.jpg",
//    "反正也是挣.jpg",
//    "天赐良缘.jpg",
//    "学有所长.jpg",
//    "宽容.png",
//    "对影成三人.jpg",
//    "小马过河.jpg",
//    "我要创业.jpg",
//    "我要当学霸.jpg",
//    "手有余箱.jpg",
//    "换位教学.jpg",
//    "文学家.jpg",
//    "有妹子远方来.jpg",
//    "朝三暮四.jpg",
//    "深不见底.jpg",
//    "焦大武馆.jpg",
//    "略知一二.jpg",
//    "第二性.jpg",
//    "等号.jpg",
//    "自学成才.jpg",
//    "衣食住情.jpg",
//    "谈面子.jpg",
//    "距离.jpg",
//    "锄禾日当午.jpg",
};


bool UserChoiceLayer::init(){
    if (!Layer::init()){
        return false;
    }
    
    mCardIndex = 0;
//    test();
    
    return true;
}

void UserChoiceLayer::test(){
    this->removeAllChildren();
    
    show(new Card(2, 3), 7, NULL);
}

void UserChoiceLayer::show(Card* card, int options, PlayerActionCallBack* callback){
    mAction = 0;
    removeAllChildren();
    if(card == NULL){
        return;
    }
    
    mPlayerActionCallBack = callback;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float posY = origin.y + visibleSize.height / 2 - 250;
    auto largeImg = createPokerFront(card, posY);
    largeImg->setTouchEnabled(false);
    
    posY -= 80;
    
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
    
    if (btn->getTag() <= 0) {
        return;
    }
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            if (mPlayerActionCallBack != NULL) {
                this->setVisible(false);
                mAction = btn->getTag();
                this->scheduleOnce(schedule_selector(UserChoiceLayer::onAction), 0.01f);
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

void UserChoiceLayer::onAction(float dt){
    if (mAction > 0) {
        mPlayerActionCallBack->onPlayerAction(mPlayer, mAction);
    }
}


cocos2d::ui::Widget* UserChoiceLayer::createPokerFront(Card* card, float posY){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto btnCard = addButton(card->getDisplay(), Size(100,150), Vec2(origin.x + visibleSize.width / 2, posY), 0);
    btnCard->loadTextures("poker_front_large.png", "", "", TextureResType::LOCAL);
    btnCard->setPosition(Vec2(origin.x + visibleSize.width / 2, posY + btnCard->getContentSize().height / 2));
    Size contentSize = btnCard->getContentSize();
    
    int imageCount = sizeof(sCardImagesFileName)/sizeof(sCardImagesFileName[0]);
    string imageName = sCardImagesFileName[mCardIndex++ % imageCount];
    
    auto img = cocos2d::ui::ImageView::create("card/" + imageName);
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

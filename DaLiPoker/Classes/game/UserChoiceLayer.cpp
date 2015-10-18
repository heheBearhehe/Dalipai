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

static string sCardImagesFileName[] = {
    "黑01-宽容.jpg",
    "黑02-第二性.jpg",
    "黑03-上课代表.jpg",
    "黑04-谈面子.jpg",
    "黑05-乌鸦喝水.jpg",
    "黑06-换位教学.jpg",
    "黑07-深不见底.jpg",
    "黑08-我要创业.jpg",
    "黑09-小马过河.jpg",
    "黑10-衣食住情.jpg",
    "黑11-自学成才.jpg",
    "黑12-距离.jpg",
    "黑13-文学家.jpg",
    "红01-学有所长.jpg",
    "红02-等号.jpg",
    "红03-天赐良缘.jpg",
    "红04-朝三暮四.jpg",
    "红05-锄禾日当午.jpg",
    "红06-偶遇.jpg",
    "红07-焦大武馆.jpg",
    "红08-我要当学霸.jpg",
    "红09-反正也是挣.jpg",
    "红10-他叫金十亿.jpg",
    "红11-友情与爱情.jpg",
    "红12-略知一二.jpg",
    "红13-对影成三人.jpg",
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
    
    show(new Card(2, 3), NULL, 7, NULL);
}

void UserChoiceLayer::show(Card* card, Card* card2, int options, PlayerActionCallBack* callback){
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
        
        if (options == Player::PLAYER_CHOICE_REMOVE_FOR_GIVE && card2 != NULL) {
            auto largeImg2 = createPokerFront(card2, posY);
            cocos2d::Vec2 oldPos = largeImg->getPosition();
            cocos2d::Size oldSize = largeImg->getContentSize();
            float scale = 0.6;
            largeImg->setScale(scale);
            largeImg2->setScale(scale);
            
            largeImg->setPosition(cocos2d::Vec2(oldPos.x - oldSize.width * scale / 2 - 10, oldPos.y));
            largeImg2->setPosition(cocos2d::Vec2(oldPos.x + oldSize.width * scale / 2 + 10, oldPos.y));
        }
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
    auto btn = cocos2d::ui::Button::create("btn_choice.png", "btn_choice_h.png", "", TextureResType::LOCAL);
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
        mPlayerActionCallBack->execute();
    }
}


cocos2d::ui::Widget* UserChoiceLayer::createPokerFront(Card* card, float posY){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto btnCard = addButton(card->getDisplay(), Size(100,150), Vec2(origin.x + visibleSize.width / 2, posY), 0);
    btnCard->loadTextures("poker_front_large.png", "", "", TextureResType::LOCAL);
    btnCard->setPosition(Vec2(origin.x + visibleSize.width / 2, posY + btnCard->getContentSize().height / 2));
    Size contentSize = btnCard->getContentSize();
    
    string imageName = getCardImageName(card);
    
    auto img = cocos2d::ui::ImageView::create("card/" + imageName);
    img->ignoreContentAdaptWithSize(false);
    img->setContentSize(Size(contentSize.width - 100, contentSize.height - 100));
    img->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2));
    btnCard->addChild(img);
    
    auto labelTitle = Label::create();
    labelTitle->setContentSize(Size(contentSize.width / 2, 50));
    labelTitle->setString(getCardImageTitle(imageName));
    labelTitle->setSystemFontSize(30);
    labelTitle->setColor(Color3B::BLACK);
    labelTitle->setPosition(Vec2(contentSize.width / 2, contentSize.height - 60));
    btnCard->addChild(labelTitle);
    
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

std::string UserChoiceLayer::getCardImageName(Card* card){
    int indexOffset = 0;
    if (card->getSuit() == SUIT::HEART || card->getSuit() == SUIT::DIAMOND) {
        indexOffset = sizeof(RANK_DISPLAY_LIST)/sizeof(RANK_DISPLAY_LIST[0]);
    }
    
    int imageCount = sizeof(sCardImagesFileName)/sizeof(sCardImagesFileName[0]);
    int index = indexOffset + card->getRank();
    if (index < 0 || index >= imageCount) {
        index = 0;
    }
    
    return sCardImagesFileName[index];
}

std::string UserChoiceLayer::getCardImageTitle(std::string imageName){
    int prefixLength = string("黑01-").length();
    int suffixLength = string(".jpg").length();
    return imageName.substr(prefixLength, imageName.length() - prefixLength - suffixLength);
}

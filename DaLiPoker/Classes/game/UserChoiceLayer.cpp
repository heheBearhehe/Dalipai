//
//  UserChoiceLayer.cpp
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#include "UserChoiceLayer.h"
#include "../model/def.h"
#include "DLUtils.h"
#include "R.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

static string sCardImagesFileName[] = {
    "黑01-图书馆，不止于读书.png",
    "黑02-第二性.png",
    "黑03-上课代表.png",
    "黑04-自学成才.png",
    "黑05-乌鸦喝水.png",
    "黑06-换位教学.png",
    "黑07-新猜谜语.png",
    "黑08-大学生创业歌.png",
    "黑09-盛宣怀办学.png",
    "黑10-反正也是挣.png",
    "黑11-智能制造.png",
    "黑12-宽容.png",
    "黑13-我要创业.png",
    "红01-交大，不止于读书.png",
    "红02-友情与爱情.png",
    "红03-天赐良缘.png",
    "红04-距离.png",
    "红05-学有所长.png",
    "红06-红绿灯.png",
    "红07-我爱江川.png",
    "红08-上有老下有小.png",
    "红09-小马过河.png",
    "红10-他叫金十亿.png",
    "红11-网girl快弟.png",
    "红12-略知一二.png",
    "红13-对影成三人.png",
    "方01-交通大学.png",
    "方02-等号.png",
    "方03-上不起.png",
    "方04-偶遇.png",
    "方05-锄禾日当午.png",
    "方06-六六大顺.png",
    "方07-听MM的话（上）.png",
    "方08-听MM的话（下）.png",
    "方09-我要延毕.png",
    "方10-有妹子远方来.png",
    "方11-碰碰车.png",
    "方12-朝三暮四.png",
    "方13-感动.png",
    "草01-创新不止于受嘲.png",
    "草02-台球.png",
    "草03-催命.png",
    "草04-谈面子.png",
    "草05-焦大武馆.png",
    "草06-修炼.png",
    "草07-文学家.png",
    "草08-画饼充饥.png",
    "草09-打台球.png",
    "草10-衣食住情.png",
    "草11-蛋画家.png",
    "草12-数绵羊.png",
    "草13-望子成凤.png",
};


static string sCardVideoUrl[] = {
    "http://www.bilibili.com/video/av5262326",
    "http://www.bilibili.com/video/av6529494",
    "http://www.bilibili.com/video/av6699322",
    "http://www.bilibili.com/video/av6709811",
    "http://www.bilibili.com/video/av6709155",
    "http://www.bilibili.com/video/av6708893",
    "http://www.bilibili.com/video/av6827828",
    "http://www.bilibili.com/video/av6710035",
    "http://www.bilibili.com/video/av6828665",
    "http://www.bilibili.com/video/av6760491/",
    "http://www.bilibili.com/video/av3711692/",
    "http://www.bilibili.com/video/av6709147",
    "http://www.bilibili.com/video/av6710169",
    "http://www.bilibili.com/video/av4888013/",
    "http://www.bilibili.com/video/av6698811",
    "http://www.bilibili.com/video/av6709317",
    "http://www.bilibili.com/video/av6529381",
    "http://www.bilibili.com/video/av6699243",
    "http://www.bilibili.com/video/av6710195",
    "http://www.bilibili.com/video/av6710236",
    "http://www.bilibili.com/video/av5426711/",
    "http://www.bilibili.com/video/av6709400",
    "http://www.bilibili.com/video/av6708963",
    "http://www.bilibili.com/video/av3709663/",
    "http://www.bilibili.com/video/av6709224",
    "http://www.bilibili.com/video/av6714050",
    "http://www.bilibili.com/video/av4383191",
    "http://www.bilibili.com/video/av6529423",
    "http://www.bilibili.com/video/av3709812/",
    "http://www.bilibili.com/video/av6697519",
    "http://www.bilibili.com/video/av6354578",
    "http://www.bilibili.com/video/av3707791/",
    "http://www.bilibili.com/video/av3711098/",
    "http://www.bilibili.com/video/av3711253/",
    "http://www.bilibili.com/video/av6713828",
    "http://www.bilibili.com/video/av6710218",
    "http://www.bilibili.com/video/av3709421/",
    "http://www.bilibili.com/video/av6416422",
    "http://www.bilibili.com/video/av3746837",
    "http://www.bilibili.com/video/av6760867/",
    "http://www.bilibili.com/video/av4737234",
    "http://www.bilibili.com/video/av3711494",
    "http://www.bilibili.com/video/av6697367",
    "http://www.bilibili.com/video/av6700257",
    "http://www.bilibili.com/video/av6708884/",
    "http://www.bilibili.com/video/av6709905",
    "http://www.bilibili.com/video/av6710234",
    "http://www.bilibili.com/video/av6409708",
    "http://www.bilibili.com/video/av6710250",
    "http://www.bilibili.com/video/av6430072",
    "http://www.bilibili.com/video/av6713691",
    "http://www.bilibili.com/video/av6710269",
};

static const int TAG_USER_CHOICE_VIEW_BASE = 1000;
static const int TAG_OPPENENT_CARD_1     = 1000;
static const int TAG_OPPENENT_CARD_2     = 1001;
static const int TAG_LARGE_CARD          = 1002;
static const int TAG_LARGE_CARD_2        = 1003;
static const int TAG_CARD_BASE           = 2000;

static const float CARD_ANIMATION_DURATION = 0.15;

bool UserChoiceLayer::init(){
    if (!Layer::init()){
        return false;
    }
    
    mCardIndex = 0;
    mTouchPause = false;
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
    
    mTouchPause = false;
    this->setTouchEnabled(true);
    mPlayerActionCallBack = callback;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float posY = origin.y + visibleSize.height / 2 - 300;
    auto largeImg = createPokerFront(card, posY);
    largeImg->setTouchEnabled(false);
    largeImg->setTag(TAG_LARGE_CARD);
    
    Widget* cardFrame = Widget::create();
    cardFrame->setTouchEnabled(true);
    cardFrame->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(UserChoiceLayer::touchEvent,this)));
    cardFrame->setTag(TAG_CARD_BASE + card->getIndex());
    cardFrame->setAnchorPoint(Vec2(0,0));
    cardFrame->setPosition(Vec2(0,0));
    cardFrame->setContentSize(largeImg->getContentSize());
    largeImg->addChild(cardFrame);
    
    posY -= 10;
    
    float buttonWidth = 200;
    float buttonHeight = 86;
    
    if(options == Player::PLAYER_CHOICE_KEEP_FOR_GIVE || options == Player::PLAYER_CHOICE_REMOVE_FOR_GIVE){
        auto btn = cocos2d::ui::Button::create("btn_choice.png", "btn_choice_h.png", "", TextureResType::LOCAL);
        btn->setUnifySizeEnabled(false);
        btn->setTag(options);
        btn->setScale9Enabled(true);
        btn->setCapInsets(Rect(50, 30, 20, 20));
        btn->setTitleText("确定");
        btn->setTouchEnabled(true);
        btn->setTitleColor(R::COLOR_TEXT_CHOICE);
        btn->setTitleFontSize(40);
        btn->setContentSize(Size(buttonWidth * 1.5, buttonHeight));
        btn->setPosition(Vec2(origin.x + visibleSize.width / 2, posY));
        btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(UserChoiceLayer::touchEvent,this)));
        this->addChild(btn, 1);
        
        if (options == Player::PLAYER_CHOICE_REMOVE_FOR_GIVE && card2 != NULL) {
            auto largeImg2 = createPokerFront(card2, posY);
            largeImg2->setTag(TAG_LARGE_CARD_2);
            largeImg2->setTouchEnabled(false);
            cocos2d::Vec2 oldPos = largeImg->getPosition();
            cocos2d::Size oldSize = largeImg->getContentSize();
            float scale = 0.7;
            largeImg->setScale(scale);
            largeImg2->setScale(scale);
            
            largeImg->setPosition(cocos2d::Vec2(oldPos.x - oldSize.width * scale / 2 - 10, oldPos.y));
            largeImg2->setPosition(cocos2d::Vec2(oldPos.x + oldSize.width * scale / 2 + 10, oldPos.y));
        }
        
    }else{
        auto btnGive = addButton("给牌", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2 - 150, posY), Player::PLAYER_CHOICE_GIVE, (options & Player::PLAYER_CHOICE_GIVE) > 0);
        btnGive->setPosition(Vec2(origin.x + visibleSize.width / 2 - btnGive->getContentSize().width - 30, posY));
        
        auto btnKeep = addButton("收牌", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2, posY), Player::PLAYER_CHOICE_KEEP, (options & Player::PLAYER_CHOICE_KEEP) > 0);
        btnKeep->setPosition(Vec2(origin.x + visibleSize.width / 2, posY));
    
        auto btnDiscard = addButton("弃牌", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2 + 150, posY), Player::PLAYER_CHOICE_DISCARD, (options & Player::PLAYER_CHOICE_DISCARD) > 0);
        btnDiscard->setPosition(Vec2(origin.x + visibleSize.width / 2 + btnDiscard->getContentSize().width + 30, posY));
    
    }
}


void UserChoiceLayer::showOppenentCard(Card* card, Card* card2){
    removeAllChildren();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float posY = origin.y + visibleSize.height / 2 - 300;
    auto largeImg = createPokerFront(card, posY);
    largeImg->setTouchEnabled(false);
    largeImg->setTag(TAG_OPPENENT_CARD_1);
    
    posY -= 80;
    if (card2 != NULL) {
        auto largeImg2 = createPokerFront(card2, posY);
        largeImg2->setTag(TAG_OPPENENT_CARD_2);
        cocos2d::Vec2 oldPos = largeImg->getPosition();
        cocos2d::Size oldSize = largeImg->getContentSize();
        float scale = 0.6;
        largeImg->setScale(scale);
        largeImg2->setScale(scale);
        
        largeImg->setPosition(cocos2d::Vec2(oldPos.x - oldSize.width * scale / 2 - 10, oldPos.y));
        largeImg2->setPosition(cocos2d::Vec2(oldPos.x + oldSize.width * scale / 2 + 10, oldPos.y));
    }
}

void UserChoiceLayer::hideOppenentCard(){
    if (mDiscardNextCardRect.size.width == 0) {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        mDiscardNextCardRect = Rect(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2, mMyNextCardRect.size.width, mMyNextCardRect.size.height);
    }
    
    Widget* card1 = (Widget *)this->getChildByTag(TAG_OPPENENT_CARD_1);
    if (card1 != NULL) {
        animationForLargeCard(card1, mDiscardNextCardRect);
    }
    Widget* card2 = (Widget *)this->getChildByTag(TAG_OPPENENT_CARD_2);
    if (card2 != NULL) {
        animationForLargeCard(card2, mDiscardNextCardRect);
    }
    
    this->scheduleOnce(schedule_selector(UserChoiceLayer::doHideOppenentCard), CARD_ANIMATION_DURATION);
}

void UserChoiceLayer::doHideOppenentCard(float dt){
    this->removeChildByTag(TAG_OPPENENT_CARD_1);
    this->removeChildByTag(TAG_OPPENENT_CARD_2);
}

cocos2d::ui::Button* UserChoiceLayer::addButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    return addButton(text, size, position, tag, true);
}

cocos2d::ui::Button* UserChoiceLayer::addButton(const std::string& text, const Size & size, const Vec2& position, int tag, bool enable){
    std::string picNormal = enable? "btn_choice.png" : "btn_choice_d.png";
    auto btn = cocos2d::ui::Button::create(picNormal, "btn_choice_h.png", "", TextureResType::LOCAL);
//    btn->setUnifySizeEnabled(false);
    btn->setTag(enable? tag : 0);
    btn->setTitleText(text);
    btn->setEnabled(enable);
    btn->setTouchEnabled(true);
    btn->setTitleColor(R::COLOR_TEXT_CHOICE);
    btn->setTitleFontSize(40);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(UserChoiceLayer::touchEvent,this)));
    this->addChild(btn, 1);
    return btn;
}


void UserChoiceLayer::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
    cocos2d::ui::Button* btn = (cocos2d::ui::Button*)ref;
    LOGI("UI. touchEvent  tag=[%d] type=[%d]", btn->getTag(), type);
    
    int tag = btn->getTag();
    if (tag <= 0 || mPaused || (tag >= TAG_USER_CHOICE_VIEW_BASE && tag < TAG_CARD_BASE)) {
        return;
    }
    
    if (mTouchPause) {
        LOGI("UI. touchEvent PAUSE!!");
        return;
    }
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            if (tag >= TAG_CARD_BASE) {
                DLUtils::openVideoUrl(getCardVideoUrl(tag - TAG_CARD_BASE));
            }else if (mPlayerActionCallBack != NULL) {
                float delayTime = 0.01;
                if(animationForAction(btn->getTag())){
                    delayTime = CARD_ANIMATION_DURATION;
                }
                mAction = btn->getTag();
                this->scheduleOnce(schedule_selector(UserChoiceLayer::onAction), delayTime);
                mTouchPause = true;
            }
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

void UserChoiceLayer::onAction(float dt){
    this->setVisible(false);
    this->setTouchEnabled(false);
    if (mAction > 0) {
        mPlayerActionCallBack->onPlayerAction(mPlayer, mAction);
        mPlayerActionCallBack->execute();
    }
}


cocos2d::ui::Widget* UserChoiceLayer::createPokerFront(Card* card, float posY){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto btnCard = addButton("", Size(100,150), Vec2(origin.x + visibleSize.width / 2, posY), 0);
    btnCard->loadTextures("poker_front_large.png", "", "", TextureResType::LOCAL);
    btnCard->setPosition(Vec2(origin.x + visibleSize.width / 2, posY + btnCard->getContentSize().height / 2 + 80));
    Size contentSize = btnCard->getContentSize();
    
    string imageName = getCardImageName(card);
    
    auto img = cocos2d::ui::ImageView::create("card/" + imageName);
    img->ignoreContentAdaptWithSize(false);
    img->setContentSize(Size(contentSize.width - 100, contentSize.height - 80));
    img->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2 + 20));
    btnCard->addChild(img);
    
    auto labelTitle = Label::create();
    labelTitle->setContentSize(Size(contentSize.width / 2, 50));
    labelTitle->setString(getCardImageTitle(imageName));
    labelTitle->setSystemFontSize(35);
    labelTitle->setColor(R::COLOR_TEXT_POKER_FRONT);
    labelTitle->setPosition(Vec2(contentSize.width / 2, labelTitle->getContentSize().height / 2 + 20));
    btnCard->addChild(labelTitle);
    
    contentSize = btnCard->getContentSize();
    float numTextSize = 60;
    float suitTextSize = 40;
    auto cardDisplay = createPokerDisplay(card, numTextSize, suitTextSize);
    cardDisplay->setPosition(Vec2(50, contentSize.height - 110));
    btnCard->addChild(cardDisplay);
    
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
    int indexOffset = sizeof(RANK_DISPLAY_LIST)/sizeof(RANK_DISPLAY_LIST[0]) * card->getSuit();
    int imageCount = sizeof(sCardImagesFileName)/sizeof(sCardImagesFileName[0]);
    int index = indexOffset + card->getRank();
    if (index < 0 || index >= imageCount) {
        index = 0;
    }
    
    return sCardImagesFileName[index];
}

std::string UserChoiceLayer::getCardImageTitle(std::string imageName){
    int prefixLength = string("黑01-").length();
    int suffixLength = string(".png").length();
    return imageName.substr(prefixLength, imageName.length() - prefixLength - suffixLength);
}

std::string UserChoiceLayer::getCardVideoUrl(int cardIndex){
    int imageCount = sizeof(sCardVideoUrl)/sizeof(sCardVideoUrl[0]);
    int suit = cardIndex % SUIT::COUNT;
    int rank = cardIndex / SUIT::COUNT;
    int index = sizeof(RANK_DISPLAY_LIST)/sizeof(RANK_DISPLAY_LIST[0]) * suit + rank;
    if (index < 0 || index >= imageCount) {
        return "";
    }
    return sCardVideoUrl[index];
}

bool UserChoiceLayer::animationForAction(int action){
    bool ret = false;
    Rect nextCardRect;
    
    if (action == Player::PLAYER_CHOICE_KEEP || action == Player::PLAYER_CHOICE_KEEP_FOR_GIVE) {
        nextCardRect = mMyNextCardRect;
    }else if(action== Player::PLAYER_CHOICE_GIVE){
        nextCardRect = mOpponentNextCardRect;
    }else if(action == Player::PLAYER_CHOICE_DISCARD || action == Player::PLAYER_CHOICE_REMOVE_FOR_GIVE ){
        nextCardRect = mDiscardNextCardRect;
    }
    
    if (nextCardRect.size.width != 0) {
        auto largeCard = this->getChildByTag(TAG_LARGE_CARD);
        if (largeCard != NULL) {
            animationForLargeCard((cocos2d::ui::Widget*)largeCard, nextCardRect);
            ret = true;
        }
        
        auto largeCard2 = this->getChildByTag(TAG_LARGE_CARD_2);
        if (largeCard2 != NULL) {
            animationForLargeCard((cocos2d::ui::Widget*)largeCard2, nextCardRect);
            ret = true;
        }
    }
    
    return ret;
}

void UserChoiceLayer::animationForLargeCard(cocos2d::ui::Widget* card, cocos2d::Rect toRect){
    if (toRect.size.width == 0) {
        return;
    }
    
    float animTime = CARD_ANIMATION_DURATION;
    ScaleTo* scaleTo = ScaleTo::create(animTime, toRect.size.width / card->getContentSize().width, toRect.size.height / card->getContentSize().height);
    MoveTo * moveTo = MoveTo::create(animTime, Vec2(toRect.origin.x, toRect.origin.y));
    FadeOut * fadein = FadeOut::create(animTime);
    Spawn* spawn = Spawn::create(moveTo, scaleTo, fadein, nullptr);
    card->runAction(spawn);
    
}




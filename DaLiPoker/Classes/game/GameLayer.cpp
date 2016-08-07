//
//  GameLayer.cpp
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#include "GameLayer.h"
#include "../model/def.h"
#include <CocosGUI.h>
#include "GameManager.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

static const int TAG_LABEL_MESSAGE   = 1000;

bool GameLayer::init(){
    if (!Layer::init()){
        return false;
    }
    
//    test();
    mShouldShowOppnentCard = false;
    initBG();
    invalidate();
    
    return true;
}

void GameLayer::setMessage(const std::string& message){
    mMessage = message;
    updateMesage();
}

void GameLayer::initBG(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto avatarMe = GameManager::getInstance()->getMyAvatar(0);
    auto avatarOppo = GameManager::getInstance()->getOppenentAvatar(0);
    
    avatarMe->setPosition(Vec2(origin.x + avatarMe->getContentSize().width / 2, origin.y + avatarMe->getContentSize().height / 2 + 20));
    avatarOppo->setPosition(Vec2(origin.x + visibleSize.width - avatarOppo->getContentSize().width / 2, origin.y + visibleSize.height - avatarMe->getContentSize().height / 2 - 10));
    
    this->addChild(avatarMe);
    this->addChild(avatarOppo);
}

void GameLayer::invalidate(){
    this->removeAllChildren();
    if (mGame == NULL) {
        return;
    }
    
    initBG();
    updateMyCard(mGame->getMyPlayerCardList());
    updateOpponentCard(mGame->getOpponentCardsList());
    updateResetCard(mGame->getResetCardsCount());
    updateDiscardCards(mGame->getDiscardCardList());
    updateDealCard();
    updateGameInfo(mGame->getMyPlayerPoints(), mGame->getOpponentPoints());
    updateMesage();
}

void GameLayer::test(){
    this->removeAllChildren();

    vector<Card*>* myCards = new vector<Card*>();
    myCards->push_back(new Card(0, 2));
    myCards->push_back(new Card(0, 3));
    myCards->push_back(new Card(3, 0));
    myCards->push_back(new Card(10, 2));
    myCards->push_back(new Card(11, 1));
    myCards->push_back(new Card(11, 1));
    myCards->push_back(new Card(12, 3));
    
    vector<Card*>* discardList = new vector<Card*>();
    discardList->push_back(new Card(0, 2));
    discardList->push_back(new Card(0, 3));
    discardList->push_back(new Card(3, 0));
    discardList->push_back(new Card(10, 1));
    discardList->push_back(new Card(6, 1));
    discardList->push_back(new Card(7, 1));
    discardList->push_back(new Card(3, 0));
    discardList->push_back(new Card(10, 2));
    discardList->push_back(new Card(11, 1));
    
    int opponentCardCount = 6;
    
    int resetCardCount = 10;
    int myPoints = 3;
    
    updateMyCard(myCards);
    updateDiscardCards(discardList);
    updateOpponentCard(discardList);
    updateResetCard(resetCardCount);
    updateGameInfo(myPoints, -1);
}

void GameLayer::update(float delta){
}

void GameLayer::drawCard(Card* card, const Vec2& position, const Size & size){
    drawText(card != NULL? card->getDisplay() : "*", position, size);
}

void GameLayer::drawText(const string& text, const Vec2& position, const Size & size){
    auto label = Label::create();
    label->setContentSize(size);
    label->setString(text);
    label->setSystemFontSize(30);
    label->setColor(R::COLOR_TEXT);
    
    label->setPosition(position);
    this->addChild(label, 1);
}

void GameLayer::updateMesage(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int posX = origin.x + visibleSize.width / 2;
    int posY = origin.y + visibleSize.height - 250;
    
    auto bg = ImageView::create("action_message.png");
    bg->setPosition(Vec2(posX, posY));
    bg->setTag(TAG_LABEL_MESSAGE);
    bg->setVisible(mMessage.size() > 0);
    
    auto label = Label::create();
    label->setContentSize(Size(100, 40));
    label->setString(mMessage);
    label->setSystemFontSize(35);
    label->setColor(R::COLOR_TEXT_ACTION_DESC);
    label->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2));
    
    this->addChild(bg, 1);
    bg->addChild(label, 1);
    
}

void GameLayer::onMessageRemoved(){
    this->removeChildByTag(TAG_LABEL_MESSAGE);
    updateMesage();
}

void GameLayer::clearMessage(){
    mMessage = "";
    if(this->getChildByTag(TAG_LABEL_MESSAGE) != NULL){
        this->removeChildByTag(TAG_LABEL_MESSAGE);
    }
    updateMesage();
}

void GameLayer::updateMyCard(vector<Card*>* cards){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = 134;
    float cardHeight = 167;
    float delta = 15;
    int posX = origin.x + 20 + cardWidth / 2 + 100;
    int posY = origin.y + 20 + cardHeight / 2 + 100;
    int visibleCardCount = 3 + 1;
    int size = (int)cards->size();
    
    Widget* card = NULL;
    for(int i = 0; i < size + 1; i++){
        if (i < size + 1 - visibleCardCount) {
            card = createPokerFront(cards->at(i), false, true);
            delta = card->getContentSize().width / 6;
        }else{
            if (i == size) {
                card = createPokerFront(NULL, false, false);
            }else{
                card = createPokerFront(cards->at(i), true, true);
            }
            delta = card->getContentSize().width / 2;
        }
        
        if (posX + 2 * cardWidth >= visibleSize.width && i < size - visibleCardCount) {
            continue;
        }
        
        card->setPosition(Vec2(posX, posY));
        this->addChild(card);
        
        posX += delta;
    }
    
    if (card != NULL) {
        card->setVisible(false);
        mMyNextCardRect = Rect(card->getPosition().x, card->getPosition().y, card->getContentSize().width, card->getContentSize().height);
    }
    
    posX += cardWidth + 20;
    
//    stringstream ss;
//    ss << cards->size();
//    drawText(ss.str(), Vec2(posX, posY), Size(cardWidth, cardHeight));
}

void GameLayer::updateOpponentCard(vector<Card*>* cards){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = 134;
    float cardHeight = 167;
    float delta = cardWidth / 3;
    int posX = origin.x + 20 + cardWidth / 2 + 100;
    int posY = origin.y + visibleSize.height - 100 - cardHeight / 2;
    int visibleCardCount = 3;
    int size = cards->size();
    
    for(int i = 0; i < size; i++){
        Widget* card;
        Card* cardToShow = NULL;
        int deltaRate;
        
        if (i < size - visibleCardCount) {
            deltaRate = 6;
        }else{
            deltaRate = 2;
        }
        cardToShow = cards->at(i);
        
        if (posX + cardWidth + 200 >= visibleSize.width && i < size - visibleCardCount) {
            continue;
        }
        
        if (shouldShowOppnentCard()) {
            card = createPokerFront(cardToShow, true, true);
        }else{
            card = createPokerBack(cardToShow);
        }
        delta = card->getContentSize().width / deltaRate;
        
        card->setPosition(Vec2(posX, posY));
        this->addChild(card);
        posX += delta;
    }
    mOpponentNextCardRect = Rect(origin.x + visibleSize.width / 2, posY, cardWidth, cardHeight);
    
    posX += delta + 20 + cardHeight;
    
    posX = visibleSize.width - 150;
    posY -= cardHeight / 2 + 10;
    
    
//    stringstream ss;
//    ss << size;
//    drawText(ss.str(), Vec2(posX, posY), Size(cardWidth, cardHeight));
}

void GameLayer::updateResetCard(int count){
    if(count == 0){
        return;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = 134;
    float cardHeight = 167;
    float delta = cardHeight / 8;
    int posX = origin.x + cardWidth / 2 + 20;
    int posY = origin.y + visibleSize.height - 400;
    
    for(int i = 0; i < count; i++){
        Widget* card = createPokerBack(NULL);
        card->setPosition(Vec2(posX, posY));
        this->addChild(card);
        
        posY -= delta;
        if (i >= 20) {
            break;
        }
    }
    
    posY -= (delta + 50);
    stringstream ss;
    ss << count;
    drawText(ss.str(), Vec2(posX, posY), Size(cardWidth, cardHeight));
}

void GameLayer::updateDiscardCards(vector<Card*>* cards){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = 80;
    float cardHeight = 120;
    int width = visibleSize.width / 3;
    int startX = origin.x + visibleSize.width / 3;
    int height = width;
    int minX = startX;
    int maxX = minX + width;
    int minY = origin.y + visibleSize.height / 2 - height / 2;
    int maxY = minY + height;
    int posX = minX;
    int posY = minY;
    int size = cards->size();
    
    Widget* card = NULL;
    for(int i = 0; i < size; i++){
        card = createPokerFront(cards->at(i), true, false);
        if (i == size - 1) {
            posX = minX + (maxX - minX) / 2;
            posY = minY + (maxY - minY) / 2 - cardHeight / 2;
        }else{
            int rotation = (i + 7) % 10;
            int x = (i + 2) % 5;
            int y = i % 3;
            posX = minX + (maxX - minX) / 5 * x + cardWidth / 2;
            posY = minY + (maxY - minY) / 3 * y;
            
            card->setRotation(36 * rotation + 10);
        }
        
        card->setPosition(Vec2(posX, posY));
        this->addChild(card);
    }
    
    if (card != NULL) {
        mDiscardNextCardRect = Rect(card->getPosition().x, card->getPosition().y, card->getContentSize().width, card->getContentSize().height);
    }
    
//    if (cards->size() > 0) {
//        posX += 20;
//        stringstream ss;
//        ss << cards->size();
//        drawText(ss.str(), Vec2(maxX + cardWidth, posY), Size(cardWidth, cardHeight));
//    }
}


void GameLayer::updateDealCard(){
    if (mDealCard == NULL) {
        return;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = visibleSize.width / 10;
    float cardHeight = cardWidth * 1.5;
    int posX = origin.x + visibleSize.width / 2;
    int posY = mDealCard->getTag() == 1 ? origin.y + 370 : origin.y + visibleSize.height - 350;
    
    Widget* card = createPokerFront(mDealCard, true, false);
    card->setPosition(Vec2(posX, posY));
    this->addChild(card);
}

//void GameLayer::updateGameInfo(int myPoints, int opponentPoints){
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//    
//    stringstream ss;
//    ss << "总分: " << myPoints;
//    
//    auto label = Label::create();
//    label->setContentSize(Size(100, 40));
//    label->setString(ss.str());
//    label->setSystemFontSize(30);
//    label->setColor(R::COLOR_TEXT);
//    label->setPosition(Vec2(origin.x + visibleSize.width - label->getContentSize().width,
//                            origin.y + label->getContentSize().height / 2 + 50));
//    this->addChild(label, 1);
//    
//    if (mShouldShowOppnentCard && opponentPoints >= 0) {
//        stringstream ss;
//        ss << "总分: " << opponentPoints;
//        
//        auto label = Label::create();
//        label->setContentSize(Size(100, 40));
//        label->setString(ss.str());
//        label->setSystemFontSize(30);
//        label->setColor(R::COLOR_TEXT);
//        label->setPosition(Vec2(origin.x + visibleSize.width - label->getContentSize().width,
//                                origin.y + label->getContentSize().height + visibleSize.height - 160));
//        this->addChild(label, 1);
//    }
//}

void GameLayer::updateGameInfo(int myPoints, int opponentPoints){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = ImageView::create("game_info_bg.png");
    Size contentSize = bg->getContentSize();
    float bottomHeight = 88;
    float marginRight = 130;
    
    {
        stringstream ss;
        //        ss << "剩余牌数: " << mGame->getResetCardsCount() << "张";
        ss << "牌数: " << mGame->getOpponentCardsList()->size() << "张";
        addGameInfoLabel(ss.str(), R::COLOR_TEXT_CARDS_INFO, Vec2(origin.x + 130 + contentSize.width / 2, origin.y + visibleSize.height - bottomHeight / 2));
    }
    
    {
        if(mGame->getPlayMode() == PLAY_MODE::REPLAY){
            stringstream ss;
            ss << "得分: " << mGame->getOpponentPoints() << "分";
            addGameInfoLabel(ss.str(), R::COLOR_TEXT_CARDS_INFO, Vec2(origin.x + visibleSize.width - marginRight - contentSize.width / 2, origin.y + visibleSize.height - bottomHeight / 2));
        }
    }
    
    {
        stringstream ss;
        ss << "牌数: " << mGame->getMyPlayerCardList()->size() << "张";
        addGameInfoLabel(ss.str(), R::COLOR_TEXT_CARDS_INFO, Vec2(origin.x + 150 + contentSize.width / 2, bottomHeight / 2));
    }
    
    {
        stringstream ss;
        ss << "得分: " << mGame->getMyPlayerPoints() << "分";
        addGameInfoLabel(ss.str(), R::COLOR_TEXT_CARDS_INFO, Vec2(origin.x + visibleSize.width - 30 - contentSize.width / 2, bottomHeight / 2));
    }
}

void GameLayer::addGameInfoLabel(const std::string& text, cocos2d::Color3B textColor, const cocos2d::Vec2& position){
    auto bg = ImageView::create("game_info_bg.png");
    bg->setPosition(position);
    auto label = Label::create();
    label->setContentSize(bg->getContentSize());
    label->setString(text);
    label->setSystemFontSize(30);
    label->setColor(textColor);
    label->setPosition(position);
    this->addChild(bg);
    this->addChild(label);
}

void GameLayer::onFinished(){
    invalidate();
    
    int oppPts = mGame->getOpponentPoints();
    stringstream ss;
    ss << "对手得分: " << oppPts << "  ";
    
    int myPts = mGame->getMyPlayerPoints();
    if (myPts < oppPts) {
        ss << "您输了";
    }else if (myPts > oppPts) {
        ss << "您赢了";
    }else{
        ss << "平局";
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    this->drawText(ss.str(), Vec2(origin.x + 200, origin.y + 50), Size(200, 50));
}


cocos2d::ui::Widget* GameLayer::createPokerFront(Card* card, bool hasDetail, bool showGiveFlag){
    auto btnCard = ImageView::create("poker_front.png");
    btnCard->ignoreContentAdaptWithSize(false);
    btnCard->setContentSize(Size(134, 167));
    
    if (card != NULL) {
        if (showGiveFlag && card->getSource() == CARD_SOURCE::GIVE) {
            auto btnCardFrame = ImageView::create("poker_front_give.png");
            btnCardFrame->ignoreContentAdaptWithSize(false);
            btnCardFrame->setContentSize(Size(134, 167));
            btnCardFrame->setPosition(Vec2(btnCard->getContentSize().width/2, btnCard->getContentSize().height/2));
            btnCard->addChild(btnCardFrame);
        }
        
        if (hasDetail) {
            Size contentSize = btnCard->getContentSize();
            float numTextSize = 30;
            float suitTextSize = 20;
            auto cardDisplay = createPokerDisplay(card, numTextSize, suitTextSize, contentSize);
            cardDisplay->setPosition(Vec2(numTextSize, contentSize.height - numTextSize * 2));
            btnCard->addChild(cardDisplay);
            
            auto btnCardSuitMain = cocos2d::ui::Button::create();
            btnCardSuitMain->setTitleText(card->getDisplaySuit());
            btnCardSuitMain->setTitleColor(Color3B::BLACK);
            btnCardSuitMain->setTitleFontSize(suitTextSize + 30);
            btnCardSuitMain->setPosition(Vec2(contentSize.width - 40, 40));
            btnCard->addChild(btnCardSuitMain);
            
            //        auto cardDisplayReverse = cardDisplay->clone();
            //        cardDisplayReverse->setRotation(180);
            //        cardDisplayReverse->setPosition(Vec2(contentSize.width - numTextSize, numTextSize * 2));
            //        btnCard->addChild(cardDisplayReverse);
        }
    }
    
    return btnCard;
}

cocos2d::ui::Widget* GameLayer::createPokerDisplay(Card* card, float numTextSize, float suitTextSize, const Size contentSize){
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

cocos2d::ui::Widget* GameLayer::createPokerBack(Card* card){
    auto btnCard = ImageView::create("poker_back.png");
    btnCard->ignoreContentAdaptWithSize(false);
    btnCard->setContentSize(Size(134, 167));
    
    if (card != NULL && card->getSource() == CARD_SOURCE::GIVE) {
        auto btnCardFrame = ImageView::create("poker_back_give.png");
        btnCardFrame->ignoreContentAdaptWithSize(false);
        btnCardFrame->setContentSize(Size(134, 167));
        btnCardFrame->setPosition(Vec2(btnCard->getContentSize().width/2, btnCard->getContentSize().height/2));
        btnCard->addChild(btnCardFrame);
    }
    
    return btnCard;
}





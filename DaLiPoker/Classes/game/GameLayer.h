//
//  GameLayer.h
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#ifndef __DaLiPoker__GameLayer__
#define __DaLiPoker__GameLayer__

#include <stdio.h>
#include <vector>

class Game;
class Card;

class GameLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(GameLayer);
    
    void setGame(Game* game) { mGame = game;}
    void setDealCardForReplay(Card* card) { mDealCard = card; }
    void setMessage(const std::string& message);
    void clearMessage();
    void invalidate();
    virtual void update(float delta);
    void onFinished();
    void setShouldShowOppnentCard(bool show) { mShouldShowOppnentCard = show; };
    bool shouldShowOppnentCard() { return mShouldShowOppnentCard; }
    cocos2d::Rect getMyNextCardRect() { return mMyNextCardRect; };
    cocos2d::Rect getOpponentNextCardRect() { return mOpponentNextCardRect; };
    cocos2d::Rect getDiscardNextCardRect() { return mDiscardNextCardRect; };
    
protected:
    void initBG();
    void updateMesage();
    void updateMyCard(std::vector<Card*>* cards);
    void updateOpponentCard(std::vector<Card*>* cards);
    void updateResetCard(int count);
    void updateDiscardCards(std::vector<Card*>* cards);
    void updateDealCard();
    void updateGameInfo(int myPoints, int opponentPoints);
    
    cocos2d::ui::Widget* createPokerFront(Card* card);
    cocos2d::ui::Widget* createPokerBack(Card* card);
    cocos2d::ui::Widget* createPokerDisplay(Card* card, float numTextSize, float suitTextSize, const cocos2d::Size contentSize);
    
    void onMessageRemoved();
    
    void drawText(const std::string& text, const cocos2d::Vec2& position, const cocos2d::Size & size);
    void drawCard(Card* card, const cocos2d::Vec2& position, const cocos2d::Size & size);
    
    void addGameInfoLabel(const std::string& text, cocos2d::Color3B textColor, const cocos2d::Vec2& position);
    
    void test();
    
protected:
    Game* mGame;
    Card* mDealCard;
    std::string mMessage;
    bool mShouldShowOppnentCard;
    cocos2d::Rect mMyNextCardRect;
    cocos2d::Rect mOpponentNextCardRect;
    cocos2d::Rect mDiscardNextCardRect;
};

#endif /* defined(__DaLiPoker__GameLayer__) */

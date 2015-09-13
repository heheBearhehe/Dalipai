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
    void setMessage(const std::string& message);
    void clearMessage();
    void invalidate();
    virtual void update(float delta);
    void onFinished();
    void setShouldShowOppnentCard(bool show) { mShouldShowOppnentCard = show; };
    bool shouldShowOppnentCard() { return mShouldShowOppnentCard; }
    
protected:
    void updateMesage();
    void updateMyCard(std::vector<Card*>* cards);
    void updateOpponentCard(std::vector<Card*>* cards);
    void updateResetCard(int count);
    void updateDiscardCards(std::vector<Card*>* cards);
    void updateGameInfo(int myPoints);
    
    cocos2d::ui::Widget* createPokerFront(Card* card);
    cocos2d::ui::Widget* createPokerBack(Card* card);
    cocos2d::ui::Widget* createPokerDisplay(Card* card, float numTextSize, float suitTextSize);
    
    void drawText(const std::string& text, const cocos2d::Vec2& position, const cocos2d::Size & size);
    void drawCard(Card* card, const cocos2d::Vec2& position, const cocos2d::Size & size);
    
    void test();
    
private:
    Game* mGame;
    std::string mMessage;
    bool mShouldShowOppnentCard;
    
};

#endif /* defined(__DaLiPoker__GameLayer__) */

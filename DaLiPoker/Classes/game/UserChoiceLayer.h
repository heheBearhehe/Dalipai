//
//  UserChoiceLayer.h
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#ifndef __DaLiPoker__UserChoiceLayer__
#define __DaLiPoker__UserChoiceLayer__

#include <stdio.h>
#include "cocos2d.h"
#include <CocosGUI.h>

class Card;
class PlayerActionCallBack;
class Player;

typedef enum buttons{
    HELP,
    STATISTIC,
    SETTINGS,
}USER_CHOICE;


class UserChoiceLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(UserChoiceLayer);
    
    void setPlayer(Player* player) { mPlayer = player; }
    void show(Card* card, Card* card2, int options, PlayerActionCallBack* callback);
    void showOppenentCard(Card* card, Card* card2);
    void hideOppenentCard();
    void setPause(bool pause) { mPaused = pause; }
    
    
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    cocos2d::ui::Button* addButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    cocos2d::ui::Button* addButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag, bool enable);
    
    void setMyNextCardRect(cocos2d::Rect cardRect)
    {
        mMyNextCardRect = cardRect;
        mMyNextCardRect.origin.y += mMyNextCardRect.size.height;
    }
    void setOpponentNextCardRect(cocos2d::Rect cardRect)
    {
        mOpponentNextCardRect = cardRect;
        mOpponentNextCardRect.origin.y -= mOpponentNextCardRect.size.height;
    }
    void setDiscardNextCardRect(cocos2d::Rect cardRect) { mDiscardNextCardRect = cardRect; };
    
protected:
    void test();
    
    cocos2d::ui::Widget* createPokerFront(Card* card, float posY);
    cocos2d::ui::Widget* createPokerDisplay(Card* card, float numTextSize, float suitTextSize);
    void onAction(float dt);
    std::string getCardImageName(Card* card);
    std::string getCardImageTitle(std::string imageName);
    
    std::string getCardVideoUrl(int cardIndex);
    
    bool animationForAction(int action);
    void animationForLargeCard(cocos2d::ui::Widget* card, cocos2d::Rect toRect);
    
    void doHideOppenentCard(float dt);
    
private:
    PlayerActionCallBack* mPlayerActionCallBack;
    Player* mPlayer;
    int mAction;
    int mCardIndex;
    bool mPaused;
    cocos2d::Rect mMyNextCardRect;
    cocos2d::Rect mOpponentNextCardRect;
    cocos2d::Rect mDiscardNextCardRect;
    bool mTouchPause;
};

#endif /* defined(__DaLiPoker__UserChoiceLayer__) */

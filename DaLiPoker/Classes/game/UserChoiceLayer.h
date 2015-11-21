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
    
    
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    cocos2d::ui::Button* addButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    
protected:
    void test();
    
    cocos2d::ui::Widget* createPokerFront(Card* card, float posY);
    cocos2d::ui::Widget* createPokerDisplay(Card* card, float numTextSize, float suitTextSize);
    void onAction(float dt);
    std::string getCardImageName(Card* card);
    std::string getCardImageTitle(std::string imageName);
    
private:
    PlayerActionCallBack* mPlayerActionCallBack;
    Player* mPlayer;
    int mAction;
    int mCardIndex;
    
};

#endif /* defined(__DaLiPoker__UserChoiceLayer__) */

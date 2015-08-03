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
    
    void show(Card* card, int options, PlayerActionCallBack* callback);
    
    
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    cocos2d::ui::Button* addButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    
protected:
    void test();
    
private:
    PlayerActionCallBack* mPlayerActionCallBack;
    
};

#endif /* defined(__DaLiPoker__UserChoiceLayer__) */

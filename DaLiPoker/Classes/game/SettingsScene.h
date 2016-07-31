//
//  SettingsScene.h
//  DaLiPoker
//
//  Created by lugan on 10/17/15.
//
//

#ifndef SettingsScene_h
#define SettingsScene_h

#include <stdio.h>

#include <stdio.h>
#include "cocos2d.h"
#include <CocosGUI.h>

class SettingsScene : public cocos2d::Scene
{
public:
    virtual bool init();
    
    CREATE_FUNC(SettingsScene);
    cocos2d::ui::Button* addButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag, bool enable);
    void drawText(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
private:
    
};


#endif /* SettingsScene_h */

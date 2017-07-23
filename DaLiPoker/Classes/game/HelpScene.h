//
//  HelpScene.h
//  DaLiPoker
//
//  Created by lugan on 8/27/16.
//
//

#ifndef HelpScene_h
#define HelpScene_h

#include <stdio.h>
#include "cocos2d.h"
#include <ui/CocosGUI.h>

class HelpScene : public cocos2d::Scene
{
public:
    virtual bool init();
    
    CREATE_FUNC(HelpScene);
    
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    
private:
    
};
#endif /* HelpScene_h */

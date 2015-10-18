#include "HelloWorldScene.h"
#include "PlayScene.h"
#include "SettingsScene.h"
#include <iostream>
using namespace std;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto bg = Sprite::create("game_bg.jpg");
    bg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    scene->addChild(bg, 0);
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->setTouchEnabled(false);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    Sprite* pSprite = Sprite::create("game_bg.jpg");
    pSprite->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(pSprite, 0);
    
    
    auto label = Label::create();
    label->setString("大李牌");
    label->setSystemFontSize(30);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);
    
    float buttonWidth = visibleSize.width / 4;
    float buttonHeight = 60;
    float buttonMargin = 20;

    addButton("开始玩牌",
              Size(buttonWidth, buttonHeight),
              Vec2(origin.x + visibleSize.width/2 - buttonWidth/2, origin.y + visibleSize.height / 2 + buttonHeight + buttonMargin),
              MAIN_BUTTONS::PLAY);
    addButton("规则介绍",
              Size(buttonWidth, buttonHeight),
              Vec2(origin.x + visibleSize.width/2 - buttonWidth/2, origin.y + visibleSize.height / 2),
              MAIN_BUTTONS::HELP);
    addButton("统计",
              Size(buttonWidth / 2, buttonHeight),
              Vec2(origin.x + visibleSize.width/2 - buttonWidth / 2, origin.y + visibleSize.height / 2 - buttonHeight - buttonMargin),
              MAIN_BUTTONS::STATISTIC);
    addButton("设置",
              Size(buttonWidth / 2, buttonHeight),
              Vec2(origin.x + visibleSize.width/2 + buttonWidth / 2, origin.y + visibleSize.height / 2 - buttonHeight - buttonMargin),
              MAIN_BUTTONS::SETTINGS);
    
    std::cout << this->getChildrenCount() << std::endl;
    return true;
}

cocos2d::ui::Button* HelloWorld::addButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create();
    btn->setTag(tag);
    btn->setTitleText(text);
    
    btn->setTouchEnabled(true);
    btn->setTitleColor(Color3B::BLACK);
    btn->setTitleFontSize(40);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(HelloWorld::touchEvent,this)));
    this->addChild(btn, 1);
    return btn;
}

void HelloWorld::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
    cocos2d::ui::Button* btn = (cocos2d::ui::Button*)ref;
    
    std::cout << btn->getTag() << " " << &type << std::endl;
    
    Vec2 beginPos = btn->getTouchBeganPosition();
    Vec2 endPos = btn->getTouchEndPosition();
    Vec2 movePos = btn->getTouchMovePosition();

    std::cout << "begin:" << beginPos.x << " " << beginPos.y << std::endl;
    std::cout << "end:"   << endPos.x   << " " << endPos.y   << std::endl;
    std::cout << "move:"  << movePos.x  << " " << movePos.y  << std::endl;
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            switch (btn->getTag()) {
                case MAIN_BUTTONS::PLAY:
                    Director::getInstance()->pushScene(PlayScene::create());
                    break;
                case MAIN_BUTTONS::HELP:
                    break;
                case MAIN_BUTTONS::STATISTIC:
                    break;
                case MAIN_BUTTONS::SETTINGS:
                    Director::getInstance()->pushScene(SettingsScene::create());
                    break;
                default:
                    break;
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

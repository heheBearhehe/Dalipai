#include "HelloWorldScene.h"
#include "PlayScene.h"
#include "SettingsScene.h"
#include "HelpScene.h"
#include "StatisticScene.h"
#include <iostream>
using namespace std;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto bg = Sprite::create("launch.jpg");
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
    
    
    Sprite* pSprite = Sprite::create("launch.jpg");
    pSprite->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(pSprite, 0);

    auto btnStart = createButton("开始游戏",
              Size(visibleSize.width/2 + 100, 120),
              Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2 - 250),
                                 MAIN_BUTTONS::PLAY);
    this->addChild(btnStart);
    
    auto btnHelp = createButton("规则介绍",
                                 Size(visibleSize.width/2 + 100, 120),
                                 Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2 - 380),
                                 MAIN_BUTTONS::HELP);
    this->addChild(btnHelp);
    
    float buttonWidth = 74;
    float buttonHeight = 74;
    float buttonMargin = 20;
    {
        auto menuBg = cocos2d::ui::ImageView::create("menu_bg.png");
        menuBg->setPosition(Vec2(origin.x + menuBg->getContentSize().width / 2, origin.y + visibleSize.height - menuBg->getContentSize().height / 2 + 500));
        
        auto btnOpen = createMenuButton("menu_open.png", Size(buttonWidth,buttonHeight), Vec2(origin.x + menuBg->getContentSize().width / 2, 60), MAIN_BUTTONS::OPEN);
        menuBg->addChild(btnOpen);
        menuBg->setTag(MAIN_BUTTONS::MENU_BG);
        this->addChild(menuBg);
    }
    
    {
        auto menuBg = cocos2d::ui::ImageView::create("menu_bg.png");
        menuBg->setPosition(Vec2(origin.x + menuBg->getContentSize().width / 2, origin.y + visibleSize.height + menuBg->getContentSize().height / 2));
        
        float posX = menuBg->getContentSize().width / 2;
        float posY = menuBg->getContentSize().height - 270;
        float gap = 130;
        
        auto btnSetting = createMenuButton("menu_settings.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY), MAIN_BUTTONS::SETTINGS);
        auto textSetting = createMenuText("设置", Vec2(posX, posY - btnHelp->getContentSize().height / 2 ));
        
        auto btnStatistic = createMenuButton("menu_stat.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY - gap), MAIN_BUTTONS::STATISTIC);
        auto textStatistic = createMenuText("统计", Vec2(posX, posY - gap - btnHelp->getContentSize().height / 2 ));
        
        auto btnClose = createMenuButton("menu_close.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY - gap * 2), MAIN_BUTTONS::CLOSE);
        
        menuBg->addChild(btnSetting);
        menuBg->addChild(btnStatistic);
        menuBg->addChild(btnClose);
        
        menuBg->addChild(textSetting);
        menuBg->addChild(textStatistic);
        
        menuBg->setTag(MAIN_BUTTONS::MENU_BG_OPENED);
        this->addChild(menuBg);
    }
    
    auto labelOwner = Label::create();
    labelOwner->setString("微信公众号:新语相声俱乐部");
    labelOwner->setSystemFontSize(30);
    labelOwner->setColor(R::COLOR_TEXT_VERSION);
    labelOwner->setPosition(Vec2(origin.x + visibleSize.width / 2, 50));
    this->addChild(labelOwner);
    
    
    std::cout << this->getChildrenCount() << std::endl;
    return true;
}

cocos2d::ui::Button* HelloWorld::createButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create("btn_choice.png", "btn_choice_h.png", "", cocos2d::ui::TextureResType::LOCAL);
//    auto btn = cocos2d::ui::Button::create();
    btn->setScale9Enabled(true);
    
    btn->setUnifySizeEnabled(true);
    btn->setTag(tag);
    btn->setTitleText(text);
    
    btn->setTouchEnabled(true);
    btn->setTitleColor(R::COLOR_TEXT_CHOICE);
    btn->setTitleFontSize(50);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(HelloWorld::touchEvent,this)));
    
    return btn;
}

cocos2d::ui::Button* HelloWorld::createMenuButton(const std::string& picPath, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create(picPath, picPath, "", cocos2d::ui::TextureResType::LOCAL);
    //    btn->setUnifySizeEnabled(false);
    btn->setTag(tag);
    btn->setCapInsets(Rect(50, 30, 20, 20));
    
    btn->setTouchEnabled(true);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(HelloWorld::touchEvent,this)));
    return btn;
}

Node* HelloWorld::createMenuText(const std::string& text, const Vec2& position){
    auto label = Label::create();
    label->setString(text);
    label->setSystemFontSize(25);
    label->setColor(R::COLOR_TEXT_MENU_TEXT);
    label->setPosition(position);
    return label;
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
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            switch (btn->getTag()) {
                case MAIN_BUTTONS::OPEN:
                {
                    auto menuBg = this->getChildByTag(MAIN_BUTTONS::MENU_BG);
                    auto menuBgOpened = this->getChildByTag(MAIN_BUTTONS::MENU_BG_OPENED);
                    
                    Size visibleSize = Director::getInstance()->getVisibleSize();
                    Vec2 origin = Director::getInstance()->getVisibleOrigin();
                    MoveTo * moveTo = MoveTo::create(0.1,
                                                     Vec2(menuBgOpened->getContentSize().width / 2,
                                                          origin.y + visibleSize.height - menuBgOpened->getContentSize().height / 2 + 200));
                    menuBgOpened->runAction(moveTo);
                    
                    menuBg->setVisible(false);
                }
                    break;
                case MAIN_BUTTONS::CLOSE:
                {
                    auto menuBg = this->getChildByTag(MAIN_BUTTONS::MENU_BG);
                    auto menuBgOpened = this->getChildByTag(MAIN_BUTTONS::MENU_BG_OPENED);
                    
                    Size visibleSize = Director::getInstance()->getVisibleSize();
                    Vec2 origin = Director::getInstance()->getVisibleOrigin();
                    MoveTo * moveTo = MoveTo::create(0.1,
                                                     Vec2(menuBgOpened->getContentSize().width / 2,
                                                          origin.y + visibleSize.height + menuBgOpened->getContentSize().height / 2));
                    menuBgOpened->runAction(moveTo);
                    menuBg->setVisible(true);
                }
                    break;
                case MAIN_BUTTONS::PLAY:
                    Director::getInstance()->pushScene(PlayScene::create());
                    break;
                case MAIN_BUTTONS::HELP:
                    Director::getInstance()->pushScene(HelpScene::create());
                    break;
                case MAIN_BUTTONS::STATISTIC:
                    Director::getInstance()->pushScene(StatisticScene::create());
                    break;
                case MAIN_BUTTONS::SETTINGS:
                    Director::getInstance()->pushScene(SettingsScene::create());
                    break;
                default:
                    break;
            }
            

            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

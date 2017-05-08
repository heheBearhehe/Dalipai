//
//  SettingsScene.cpp
//  DaLiPoker
//
//  Created by lugan on 10/17/15.
//
//

#include "SettingsScene.h"
#include "Utils.h"
#include "Settings.h"
#include "GameManager.h"
#include "../model/def.h"
#include "DLUtils.h"
#include "AboutScene.h"

using namespace std;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
USING_NS_CC;

static const int HEADER_HEIGHT = 110;
static const int ITEM_HEIGHT   = 110;

static const int TAG_BUTTON_BACK        = 1000;
static const int TAG_DIALOG_PLAYER      = 1001;
static const int TAG_DIALOG_CONTENT     = 1002;
static const int TAG_PLAYER_SELECTED    = 1003;
static const int TAG_PLAYER_DESC        = 1004;

static const int TAG_ACTION_CHOOSE_OPPNENT       = 2000;
static const int TAG_ACTION_CHOOSE_MY_AVATAR     = 2001;
static const int TAG_ACTION_CHOOSE_WHO_IS_FIRST  = 2002;
static const int TAG_ACTION_MUSIC                = 2003;
static const int TAG_ACTION_SOUND_EFFECT         = 2004;
static const int TAG_ACTION_CONTACT_US           = 2005;
static const int TAG_ACTION_ABOUT                = 2006;
static const int TAG_ACTION_PLAYER_SAVE          = 2007;
static const int TAG_ACTION_DIALOG_BG            = 2008;

static const int TAG_ACTION_AVATAR_BASE          = 3000;




static string sAvatarDesc[] = {
    "彩蛋\n随机抽取一位对手与您对战，惊喜总在不经意间~",
    "郭老师\n呆萌的对手，个性内向，对胜负无欲无求，无论说相声还是玩儿牌都能给你带来最大的乐趣！",
    "大米\n一般的对手，玩牌水平起伏不定，和她玩牌你必须做好应付高手的准备，才能享受虐菜的快感。",
    "大雄\n普通的对手，打牌时听天由命，赢了他也别骄傲，输了他……也可以换个人试试。",
    "大李\n激进的对手，急性子中的战斗急！想方设法地要控制对手的得分，至于自己得分多少？只要能让对手不爽就够了。",
    "钰姐\n高深的对手，步步为营，绵里藏针，手机AI根本不足以表现她的可怕，你和真人玩一把才会懂。",
};


bool SettingsScene::init(){
    if (!Scene::init()){
        return false;
    }
    
    mCharactorDesc = NULL;
    for(int i = 0; i < CHARACTOR_COUNT; i++){
        mCharactorFrame[i] = NULL;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    GameManager::getInstance()->initTopBar(this, "设置");
    
    int posY = origin.y + visibleSize.height - HEADER_HEIGHT / 2;
    
    auto oppentAvatar = createAvatar(Settings::getInstance()->opponentCharacter, true);
    auto myAvatar = createAvatar(Settings::getInstance()->myAvatar, false);
    
    auto whoIsFirst = createLabel(getFirstPlayerDesc(Settings::getInstance()->firstPlayer));
    
    auto musicSwitcher = getSwitcher(Settings::getInstance()->backgroundMusic, TAG_ACTION_MUSIC);
    auto soundSwitcher = getSwitcher(Settings::getInstance()->soundEffect, TAG_ACTION_SOUND_EFFECT);
    
    this->addItem("对手", TAG_ACTION_CHOOSE_OPPNENT, oppentAvatar)->setPosition(Vec2(origin.x + visibleSize.width / 2, posY - ITEM_HEIGHT));
    this->addItem("自己", TAG_ACTION_CHOOSE_MY_AVATAR, myAvatar)->setPosition(Vec2(origin.x + visibleSize.width / 2, posY - ITEM_HEIGHT * 2));
    this->addItem("先手", TAG_ACTION_CHOOSE_WHO_IS_FIRST, whoIsFirst)->setPosition(Vec2(origin.x + visibleSize.width / 2, posY - ITEM_HEIGHT * 3));
    this->addItem("背景音乐", 0, musicSwitcher)->setPosition(Vec2(origin.x + visibleSize.width / 2, posY - ITEM_HEIGHT * 4));
    this->addItem("音效", 0, soundSwitcher)->setPosition(Vec2(origin.x + visibleSize.width / 2, posY - ITEM_HEIGHT * 5));
    this->addItem("联系我们", TAG_ACTION_CONTACT_US, NULL)->setPosition(Vec2(origin.x + visibleSize.width / 2, posY - ITEM_HEIGHT * 6));
    this->addItem("关于", TAG_ACTION_ABOUT, NULL)->setPosition(Vec2(origin.x + visibleSize.width / 2, posY - ITEM_HEIGHT * 7));

    mDialog = createPlayerDialog();
    this->addChild(mDialog);
    
    
    auto labelVersion = Label::create();
    labelVersion->setString("版本: " + DL_APP_VERSION);
    labelVersion->setSystemFontSize(30);
    labelVersion->setColor(R::COLOR_TEXT_VERSION);
    labelVersion->setPosition(Vec2(origin.x + visibleSize.width / 2, 50));
    this->addChild(labelVersion);
    
    return true;
}

std::string SettingsScene::getFirstPlayerDesc(int firstPlayer){
    switch (firstPlayer) {
        case GAME_FIRST_PLAYER::RANDOM:
            return "随机";
        case GAME_FIRST_PLAYER::PLAYER_1:
            return "自己";
        case GAME_FIRST_PLAYER::PLAYER_2:
            return "对方";
            
        default:
            break;
    }
    return "";
}

Widget* SettingsScene::createAvatar(int charactor, bool oppnent){
    Widget* avatarFrame = Widget::create();
    
    ImageView* avatarImageView = NULL;
    int width = 0;
    string name;
    if (charactor != 0) {
        if (oppnent) {
            avatarImageView = GameManager::getInstance()->getOppenentAvatar(charactor, AVATAR_STATUS_NORMAL);
        }else{
            avatarImageView = GameManager::getInstance()->getMyAvatar(charactor, AVATAR_STATUS_NORMAL);
        }
    }else{
        avatarImageView = cocos2d::ui::ImageView::create("settings_random.png");
        avatarImageView->ignoreContentAdaptWithSize(false);
    }
    avatarImageView->setContentSize(Size(80, 80));
    avatarImageView->setPosition(Vec2(avatarImageView->getContentSize().width / 2, ITEM_HEIGHT / 2 + 5));
    width += avatarImageView->getContentSize().width + 10;
    
    if (oppnent) {
        name = GameManager::getInstance()->getOppnentCharactorName(charactor);
    }else{
        name = GameManager::getInstance()->getMyAvatarName(charactor);
    }

    auto nameLabel = createLabel(name);
    nameLabel->setColor(R::COLOR_TEXT_CHARACTOR_DESC);
    nameLabel->setContentSize(Size(name.size() * 30, ITEM_HEIGHT));
    nameLabel->setPosition(Vec2(width + nameLabel->getContentSize().width / 2, ITEM_HEIGHT / 2));
    width += nameLabel->getContentSize().width;
    
    if (avatarImageView != NULL) {
        avatarFrame->addChild(avatarImageView);
    }
    avatarFrame->addChild(nameLabel);
    avatarFrame->setContentSize(Size(width, ITEM_HEIGHT));
    
    return avatarFrame;
}

cocos2d::Label* SettingsScene::createLabel(const std::string& text){
    auto label = Label::create();
    label->setString(text);
    label->setSystemFontSize(40);
    label->setColor(R::COLOR_TEXT_CHARACTOR_DESC);
    return label;
}

cocos2d::ui::Button* SettingsScene::addButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create();
    btn->setColor(R::COLOR_TEXT);
    btn->ignoreAnchorPointForPosition(true);
    btn->setTag(tag);
    btn->setTitleText(text);
    btn->setTouchEnabled(true);
    btn->setTitleColor(R::COLOR_TEXT_CHOICE);
    btn->setTitleFontSize(40);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(SettingsScene::touchEvent,this)));
    this->addChild(btn, 1);
    return btn;
}

cocos2d::ui::Widget* SettingsScene::getSwitcher(bool selected, int tag){
    std::string picName = selected? "settings_switch_on.png" : "settings_switch_off.png";
    cocos2d::ui::Button* switcher = cocos2d::ui::Button::create(picName, picName, picName, TextureResType::LOCAL);
    switcher->setContentSize(Size(ITEM_HEIGHT, ITEM_HEIGHT));
    switcher->setAnchorPoint(Vec2(0.5, 0.51));
    switcher->setTouchEnabled(true);
    switcher->setTag(tag);
    switcher->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(SettingsScene::touchEvent,this)));
    return (cocos2d::ui::Widget*)switcher;
}

void SettingsScene::onClickSwitcher(cocos2d::ui::Widget* switcher){
    switcher->setEnabled(!switcher->isEnabled());
}

cocos2d::ui::Widget* SettingsScene::addItem(const std::string& title, int actionTag, cocos2d::Node* rightItem){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::ui::Widget* item = Widget::create();
    item->setContentSize(Size(visibleSize.width, ITEM_HEIGHT));
    item->setTouchEnabled(actionTag > 0);
    
    auto label = Label::create();
    label->setContentSize(Size(200, ITEM_HEIGHT));
    label->setAnchorPoint(Vec2(0, 0.5));
    label->setPosition(Vec2(30, ITEM_HEIGHT/2));
    label->setString(title);
    label->setSystemFontSize(40);
    label->setColor(R::COLOR_TEXT_SETTINGS);
    item->addChild(label);
    
    bool hasMore = actionTag > 0 && actionTag != TAG_ACTION_MUSIC && actionTag != TAG_ACTION_SOUND_EFFECT;
    
    int rightItemOffset = 40;
    if (hasMore) {
        auto rightArrow = cocos2d::ui::ImageView::create("settings_more.png");
        rightArrow->ignoreContentAdaptWithSize(true);
        rightArrow->setContentSize(Size(ITEM_HEIGHT, ITEM_HEIGHT));
        rightArrow->setAnchorPoint(Vec2(0, 0.5));
        rightArrow->setPosition(Vec2(item->getContentSize().width - 60, ITEM_HEIGHT / 2));
        item->addChild(rightArrow);
        rightItemOffset = ITEM_HEIGHT;
    }
    
    item->setTag(actionTag);
    item->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(SettingsScene::touchEvent,this)));
    
    if (rightItem != NULL) {
        rightItem->setPosition(Vec2(item->getContentSize().width - rightItemOffset - rightItem->getContentSize().width / 2, ITEM_HEIGHT / 2));
        item->addChild(rightItem);
    }
    
    auto dividerLine = DrawNode::create();
    dividerLine->setContentSize(Size(item->getContentSize().width, 2));
    dividerLine->setPosition(Vec2(0, 0));
    dividerLine->drawLine(Vec2(0,0), Vec2(item->getContentSize().width, 0), Color4F(R::COLOR_SETTINGS_DIVIDER));
    
    
    item->addChild(dividerLine);
    
    this->addChild(item);
    return item;
}

cocos2d::Layer* SettingsScene::createPlayerDialog(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int btnHeight = 90;
    
    Layer* dialog = LayerColor::create();
    dialog->setColor(Color3B(0,0,0));
    dialog->setOpacity(128);
    dialog->setContentSize(visibleSize);
    dialog->setVisible(false);
    
    auto dialogBgTouch = Layout::create();
    dialogBgTouch->setTag(TAG_DIALOG_PLAYER);
    dialogBgTouch->setContentSize(visibleSize);
    dialogBgTouch->setAnchorPoint(Vec2(0.5, 0.5));
    dialogBgTouch->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    dialogBgTouch->setTouchEnabled(true);
    dialogBgTouch->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(SettingsScene::touchEvent,this)));
    
    auto dialogBg = Layout::create();
    dialogBg->setContentSize(Size(visibleSize.width - 50, 800));
    dialogBg->setAnchorPoint(Vec2(0.5, 0.5));
    dialogBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    dialogBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    dialogBg->setBackGroundColor(R::COLOR_DIALOG_BG);
    dialogBg->setTag(TAG_ACTION_DIALOG_BG);
    dialogBg->setTouchEnabled(true);
    dialogBg->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(SettingsScene::touchEvent,this)));
    
    
    auto dialogContent = Layout::create();
    dialogContent->setContentSize(Size(dialogBg->getContentSize().width - 40, dialogBg->getContentSize().height - 40 - btnHeight));
    dialogContent->setAnchorPoint(Vec2(0.5, 0.5));
    dialogContent->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + btnHeight / 2));
    dialogContent->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    dialogContent->setBackGroundColor(R::COLOR_SETTINGS_DIVIDER);
    dialogContent->setTag(TAG_DIALOG_CONTENT);
    
    int avatarFrameWidth = (dialogContent->getContentSize().width - 20) / 3;
    Size avatarFrameSize = Size(avatarFrameWidth, avatarFrameWidth);
    int posX = 10;
    int posY = dialogContent->getContentSize().height - avatarFrameSize.height + 10;
    int offset = 50;
    
    int selectedCharactor = Settings::getInstance()->opponentCharacter;
    dialogContent->addChild(createAvatarInDialog(1, Vec2(posX, posY), avatarFrameSize, selectedCharactor == 1));
    dialogContent->addChild(createAvatarInDialog(2, Vec2(posX + avatarFrameWidth, posY), avatarFrameSize, selectedCharactor == 2));
    dialogContent->addChild(createAvatarInDialog(3, Vec2(posX + avatarFrameWidth * 2, posY), avatarFrameSize, selectedCharactor == 3));
    dialogContent->addChild(createAvatarInDialog(4, Vec2(posX, posY - avatarFrameWidth + offset), avatarFrameSize, selectedCharactor == 4));
    dialogContent->addChild(createAvatarInDialog(5, Vec2(posX + avatarFrameWidth, posY - avatarFrameWidth + offset), avatarFrameSize, selectedCharactor == 5));
    dialogContent->addChild(createAvatarInDialog(0, Vec2(posX + avatarFrameWidth * 2, posY - avatarFrameWidth + offset), avatarFrameSize, selectedCharactor == 0));
    
    posY = dialogContent->getContentSize().height - 2 * avatarFrameSize.height + 10;
    auto dividerLine = DrawNode::create();
    dividerLine->setContentSize(Size(dialogContent->getContentSize().width, 2));
    dividerLine->setPosition(Vec2(0, posY + 60));
    dividerLine->drawLine(Vec2(0,0), Vec2(dialogContent->getContentSize().width, 0), Color4F(0,0,0,0.26));
    dialogContent->addChild(dividerLine);
    
    auto labelDesc = Label::create();
    labelDesc->setAnchorPoint(Vec2(0.5,0));
    labelDesc->setContentSize(Size(dialogContent->getContentSize().width, posY));
    labelDesc->setPosition(Vec2(dialogContent->getContentSize().width / 2, 0));
    labelDesc->setSystemFontSize(40);
    labelDesc->setAlignment(TextHAlignment::LEFT);
    labelDesc->setDimensions(dialogContent->getContentSize().width - 60, posY);
    labelDesc->setColor(R::COLOR_TEXT_CARDS_TOTAL);
    labelDesc->setString(sAvatarDesc[Settings::getInstance()->opponentCharacter]);
    mCharactorDesc = labelDesc;
    dialogContent->addChild(labelDesc);
    
    auto btnOk = Layout::create();
    btnOk->setContentSize(Size(dialogContent->getContentSize().width, btnHeight));
    btnOk->setAnchorPoint(Vec2(0, 0));
    btnOk->setPosition(Vec2(20, 10));
    btnOk->setTag(TAG_ACTION_PLAYER_SAVE);
    btnOk->setTouchEnabled(true);
    btnOk->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(SettingsScene::touchEvent,this)));
    
    auto btnOkHighlight = Layout::create();
    btnOkHighlight->setContentSize(btnOk->getContentSize());
    btnOkHighlight->setAnchorPoint(Vec2(0.5, 0.5));
    btnOkHighlight->setPosition(Vec2(btnOk->getContentSize().width / 2, btnOk->getContentSize().height / 2));
    btnOkHighlight->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    btnOkHighlight->setBackGroundColor(R::COLOR_DIALOG_BTN_H);
    btnOkHighlight->setVisible(false);
    mBtnH = btnOkHighlight;
    btnOk->addChild(btnOkHighlight);
    
    dialogBg->addChild(btnOk);
    
    auto labelSave = Label::create();
    labelSave->setString("保存");
    labelSave->setSystemFontSize(45);
    labelSave->setColor(R::COLOR_TEXT_DIALOG_ACTION);
    labelSave->setPosition(Vec2(btnOk->getContentSize().width / 2, btnOk->getContentSize().height / 2));
    btnOk->addChild(labelSave);
    
    dialog->addChild(dialogBgTouch);
    dialog->addChild(dialogBg);
    dialog->addChild(dialogContent);
    
    return dialog;
}

cocos2d::ui::Widget* SettingsScene::createAvatarInDialog(int charactor, Vec2 position, Size size, bool selected){
    Widget* avatarFrame = Widget::create();
    ImageView* avatarImageView = NULL;
    
    mCharactorFrame[charactor] = avatarFrame;
    avatarFrame->setAnchorPoint(Vec2(0, 0));
    avatarFrame->setPosition(position);
    avatarFrame->setContentSize(size);
    avatarFrame->setTag(TAG_ACTION_AVATAR_BASE + charactor);
    avatarFrame->setTouchEnabled(true);
    avatarFrame->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(SettingsScene::touchEvent,this)));
    
    Vec2 pos = Vec2(avatarFrame->getContentSize().width / 2, avatarFrame->getContentSize().height / 2);
    
    ImageView* bg;
    if (selected) {
        bg = ImageView::create("settings_avatar_bg_s.png");
    }else{
        bg = ImageView::create("settings_avatar_bg_n.png");
    }
    bg->setPosition(pos);
    
    avatarFrame->addChild(bg);
    
    if (charactor != 0) {
        avatarImageView = GameManager::getInstance()->getOppenentAvatar(charactor, AVATAR_STATUS_NORMAL);
    }else{
        avatarImageView = cocos2d::ui::ImageView::create("settings_random.png");
        avatarImageView->ignoreContentAdaptWithSize(false);
    }
    avatarImageView->setContentSize(Size(size.width - 80, size.height - 80));
    avatarImageView->setPosition(pos);
    avatarFrame->addChild(avatarImageView);

    return avatarFrame;
}

void SettingsScene::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
    cocos2d::ui::Button* btn = (cocos2d::ui::Button*)ref;
    LOGI("UI. touchEvent  tag=[%d]", btn->getTag());
    
    if (btn->getTag() <= 0) {
        return;
    }
    
    if (mDialog->isVisible()) {
        if (btn->getTag() != TAG_ACTION_PLAYER_SAVE && btn->getTag() < TAG_ACTION_AVATAR_BASE) {
            return;
        }
    }
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            if (btn->getTag() == TAG_ACTION_PLAYER_SAVE) {
                mBtnH->setVisible(true);
            }
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            mBtnH->setVisible(false);
            if(btn->getTag() == TAG_ACTION_CHOOSE_OPPNENT){
                mDialog->setVisible(true);
            }else if(btn->getTag() == TAG_ACTION_CHOOSE_MY_AVATAR){
                int avatar = Settings::getInstance()->myAvatar;
                if (++avatar > 2) {
                    avatar = 0;
                }
                Settings::getInstance()->myAvatar = avatar;
                Settings::getInstance()->save();
                
                invalidate();
            }else if(btn->getTag() == TAG_ACTION_CHOOSE_WHO_IS_FIRST){
                int firstPlayer = Settings::getInstance()->firstPlayer;
                if (++firstPlayer > 2) {
                    firstPlayer = 0;
                }
                Settings::getInstance()->firstPlayer = firstPlayer;
                Settings::getInstance()->save();
                
                invalidate();
            }else if(btn->getTag() == TAG_ACTION_PLAYER_SAVE){
                mDialog->setVisible(false);
                invalidate();
            }else if(btn->getTag() == TAG_ACTION_MUSIC){
                Settings::getInstance()->backgroundMusic = !Settings::getInstance()->backgroundMusic;
                Settings::getInstance()->save();
                invalidate();
                GameManager::getInstance()->playBackgroudMusic();
            }else if(btn->getTag() == TAG_ACTION_SOUND_EFFECT){
                Settings::getInstance()->soundEffect = !Settings::getInstance()->soundEffect;
                Settings::getInstance()->save();
                invalidate();
            }else if(btn->getTag() == TAG_ACTION_CONTACT_US){
                DLUtils::sendEmail();
            }else if(btn->getTag() == TAG_ACTION_ABOUT){
                Director::getInstance()->pushScene(AboutScene::create());
            }else{
                int charactor = btn->getTag() - TAG_ACTION_AVATAR_BASE;
                if (charactor >= 0 && charactor < CHARACTOR_COUNT) {
                    Settings::getInstance()->opponentCharacter = charactor;
                    Settings::getInstance()->save();
                    invalidate();
                    mDialog->setVisible(true);
                    
                    GameManager::getInstance()->playBackgroudMusic();
                }
            }
            
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            mBtnH->setVisible(false);
            break;
            
        default:
            break;
    }
}


void SettingsScene::invalidate(){
    this->removeAllChildren();
    this->init();
}


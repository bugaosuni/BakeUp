#include "Base.h"
#include "SettingsDialog.h"
#include "DailyTaskDialog.h"
#include "AchievementDialog.h"
#include "DataMgr.h"

Base::Base()
{
    m_view = NULL;
}

Base::~Base()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    cocos2d::network::HttpClient::destroyInstance();
}

bool Base::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    m_listener = EventListenerKeyboard::create(); 
    
    AudioEnginMgr::getInstance()->playBackgroundMusic();

    DataMgr::bIsGameScene = false;
    return true;
}

void Base::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent) 
{ 
    
} 

Size Base::getVisableSize()
{
    return Director::getInstance()->getVisibleSize();
}

void Base::setLabelText(Widget* widget, string text)
{
    TextBMFont* fontBmf = dynamic_cast<TextBMFont*>(widget);
    if (fontBmf == NULL)
    {
        Text* font = dynamic_cast<Text*>(widget);
        if (font == NULL)
        {
            return ;
        }
        font->setText(text);
    }
    else
    {
        fontBmf->setText(text);
    }
}

void Base::setLabelText(Widget* widget, int text)
{
    char ch[128];
    sprintf(ch, "%d", text);
    setLabelText(widget, ch);
}

void Base::setLabelText(Node* node, string text)
{
    LabelBMFont* fontBmf = dynamic_cast<LabelBMFont*>(node);
    if (fontBmf == NULL)
    {
        Label* font = dynamic_cast<Label*>(node);
        if (font == NULL)
        {
            return ;
        }
        font->setString(text);
    }
    else
    {
        fontBmf->setString(text);
    }
}

void Base::setLabelText(Node* node, int text)
{
    char ch[128];
    sprintf(ch, "%d", text);
    setLabelText(node, ch);
}

void Base::remove()
{
    DataMgr::modalShow = false;
    this->removeFromParentAndCleanup(true);
}

void Base::settingsCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            AudioEnginMgr::getInstance()->playBtnEffect();
            DataMgr::modalShow = true;
            SettingsDialog* dlg = new SettingsDialog();
            dlg->init();
            this->addChild(dlg, 999);
            dlg->release();
        }
        break;
    default:
        break;
    }
}

void Base::dailyTaskCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
    {
        DailyTaskDialog* dlg = new DailyTaskDialog();
        dlg->init();
        this->addChild(dlg, 999);
        dlg->release();
    }
    break;
    default:
        break;
    }
}

void Base::achievementCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
    {
        AchievementDialog* dlg = new AchievementDialog();
        dlg->init();
        this->addChild(dlg, 999);
        dlg->release();
    }
    break;
    default:
        break;
    }
}

void Base::addOwnAds()
{
    auto adConfData = UtilHelper::readAdConfData();
    auto path = FileUtils::getInstance()->getWritablePath();
    Vector<MenuItem*> vec;
    vec.clear();
    for (int i = 0; i < adConfData.size(); i++)
    {
        auto icon = path + adConfData[i].icon;
        if (FileUtils::getInstance()->isFileExist(icon))
        {
            auto package = adConfData[i].package;
            auto menuItem = MenuItemImage::create(icon, 
                icon,  
                [package](Ref *pSender)  
            {  
                UtilHelper::showOwnAds(4, package);
            });
            vec.pushBack(menuItem);
        }
    }
    if (!vec.empty())
    {
        auto menu = Menu::createWithArray(vec);
        menu->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, 50));
        menu->alignItemsHorizontallyWithPadding(30.0f);
        this->addChild(menu, 10);
    }
}

void Base::refreshCoins()
{
    auto labelGold = Helper::seekWidgetByName(m_view, "BitmapLabel_Gold");
    if (labelGold != NULL)
    {
        setLabelText(labelGold, UtilHelper::getFromInteger(USER_GOLD));
    }
}
#include "UpgradeCarScene.h"
#include "UtilHelper.h"
#include "SelectCarScene.h"
#include "GameScene.h"
#include "DataMgr.h"
#include "ConfirmDialog.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "MobClickCpp.h"
#endif

Scene* UpgradeCarScene::createScene()
{
    auto scene = Scene::create();
    auto layer = UpgradeCarScene::create();
    scene->addChild(layer);
    return scene;
}

UpgradeCarScene::UpgradeCarScene()
{
    m_labelGold = nullptr;
    m_bitmapLabelUpgrade = nullptr;
    m_labelDesc = nullptr;
    m_bitmapLabelCost = nullptr;
    m_currType = e_type1;
    m_partData.clear();
    m_upgradeTypes.clear();
    m_buttonUpgrade = nullptr;
    int m_upgradeCost = 0;
}

UpgradeCarScene::~UpgradeCarScene()
{
}

bool UpgradeCarScene::init()
{
    if ( !Base::init() )
    {
        return false;
    }

    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/UpgradeCarScene.ExportJson");
    m_view->setSize(getVisableSize());
    m_view->setPosition(VisibleRect::leftBottom());
    this->addChild(m_view);

    initPartData();
    initUI();
    initClicks();
    
    m_listener->onKeyReleased = CC_CALLBACK_2(UpgradeCarScene::onKeyReleased, this); 
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener, this); 

    return true;
}
void UpgradeCarScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    if(EventKeyboard::KeyCode::KEY_RETURN == keycode || EventKeyboard::KeyCode::KEY_ESCAPE == keycode || EventKeyboard::KeyCode::KEY_BACKSPACE == keycode)
    {
        if (!DataMgr::modalShow)
        {
            Director::getInstance()->replaceScene(TransitionSlideInL::create(0.5f, SelectCarScene::createScene()));
        }
    }
}
void UpgradeCarScene::initUI()
{
    auto bitmapLabelTitle = Helper::seekWidgetByName(m_view, "BitmapLabel_Title");
    setLabelText(bitmapLabelTitle, DataMgr::getInstance()->getTextData()["upgrade_title"].c_str());

    //label gold
    m_labelGold = Helper::seekWidgetByName(m_view, "BitmapLabel_Gold");
    setLabelText(m_labelGold, UtilHelper::getFromInteger(USER_GOLD));

    //daily task
    auto buttonAdd = Helper::seekWidgetByName(m_view, "Button_DailyTask");
    buttonAdd->addTouchEventListener(this, toucheventselector(UpgradeCarScene::dailyTaskCallback));
    
    auto achievementTask = Helper::seekWidgetByName(m_view, "Button_Achievement");
    achievementTask->addTouchEventListener(this, toucheventselector(UpgradeCarScene::achievementCallback));

    //settings
    auto buttonSettings = Helper::seekWidgetByName(m_view, "Button_Settings");
    buttonSettings->addTouchEventListener(this, toucheventselector(UpgradeCarScene::settingsCallback));

    //back
    auto buttonBack = Helper::seekWidgetByName(m_view, "Button_Back");
    buttonBack->addTouchEventListener(this, toucheventselector(UpgradeCarScene::backCallback));

    //go
    auto buttonGo = Helper::seekWidgetByName(m_view, "Button_Go");
    buttonGo->addTouchEventListener(this, toucheventselector(UpgradeCarScene::goCallback));

    //panel_desc
    auto panelPart = Helper::seekWidgetByName(m_view, "Panel_Part");

    //panel_desc
    auto panelDesc = Helper::seekWidgetByName(m_view, "Panel_Desc");
    auto size = panelDesc->getSize();

    //upgrade type
    m_bitmapLabelUpgrade = Helper::seekWidgetByName(m_view, "BitmapLabel_Upgrade");
    setLabelText(m_bitmapLabelUpgrade, DataMgr::getInstance()->getTextData()[m_partData[e_type1].name.c_str()].c_str());

    //upgrade desc
    char ch[32];
    sprintf(ch, "%s_desc", m_partData[e_type1].name.c_str());
    m_labelDesc = Label::createWithBMFont("UI/font/number_24.fnt", DataMgr::getInstance()->getTextData()[ch].c_str(), TextHAlignment::LEFT, size.width * 0.94f);
    m_labelDesc->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    m_labelDesc->setPosition(size.width * 0.03f, size.height * 0.7f);
    panelDesc->addChild(m_labelDesc);

    //button upgrade
    m_buttonUpgrade = Helper::seekWidgetByName(m_view, "Button_Upgrade");
    m_buttonUpgrade->addTouchEventListener(this, toucheventselector(UpgradeCarScene::upgradeCallback));

    //upgrade cost
    m_bitmapLabelCost = Helper::seekWidgetByName(m_view, "BitmapLabel_Cost");
    auto carid = DataMgr::carIndex + 1;
    auto level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part1_level - 1;
    if ((level + 1) >= DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part1_maxlevel)
    {
        setLabelText(m_bitmapLabelCost, DataMgr::getInstance()->getTextData()["max_level"].c_str());
        m_buttonUpgrade->setEnabled(false);
        m_buttonUpgrade->setBright(false);
    }
    else
    {
        m_upgradeCost = DataMgr::getInstance()->getPartCostData()[carid][level].part1_upgrade_cost;
        setLabelText(m_bitmapLabelCost, m_upgradeCost);
        m_buttonUpgrade->setEnabled(true);
        m_buttonUpgrade->setBright(true);
    }

    addOwnAds();
}

void UpgradeCarScene::initPartData()
{
    m_partData.clear();
    PartData data;
    data.name = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part1_name;
    data.level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part1_level;
    data.maxlevel = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part1_maxlevel;
    m_partData.push_back(data);

    data.name = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part2_name;
    data.level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part2_level;
    data.maxlevel = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part2_maxlevel;
    m_partData.push_back(data);

    data.name = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part3_name;
    data.level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part3_level;
    data.maxlevel = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part3_maxlevel;
    m_partData.push_back(data);

    data.name = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part4_name;
    data.level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part4_level;
    data.maxlevel = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part4_maxlevel;
    m_partData.push_back(data);
}

void UpgradeCarScene::initClicks()
{
    m_upgradeTypes.clear();
    char ch[32];
    for (int i = 0; i < e_type_count; i++)
    {
        sprintf(ch, "Button_Part%d", i + 1);
        Button* buttonPart = dynamic_cast<Button*>(Helper::seekWidgetByName(m_view, ch));
        buttonPart->addTouchEventListener(this, toucheventselector(UpgradeCarScene::clickCallback));
        m_upgradeTypes.push_back(buttonPart);

        sprintf(ch, "icon_%s.png", m_partData[i].name.c_str());
        Sprite* sprite = Sprite::createWithSpriteFrameName(ch);
        if (sprite)
        {
            Size size = buttonPart->getSize();
            sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprite->setPosition(Vec2(size.width / 2, size.height / 2));
            buttonPart->addChild(sprite);
        }

        sprintf(ch, "BitmapLabel_Level%d", i + 1);
        auto levelPart = Helper::seekWidgetByName(m_view, ch);
        auto levelText = DataMgr::getInstance()->getTextData()["upgrade_level"];
        sprintf(ch, levelText.c_str(), m_partData[i].level, m_partData[i].maxlevel);
        setLabelText(levelPart, ch);
    }
    m_upgradeTypes[m_currType]->setBright(false);
}

void UpgradeCarScene::refreshDesc()
{
    //DESC
    char ch[32];
    sprintf(ch, "%s_desc", m_partData[m_currType].name.c_str());
    setLabelText(m_bitmapLabelUpgrade, DataMgr::getInstance()->getTextData()[m_partData[m_currType].name.c_str()].c_str());
    setLabelText(m_labelDesc, DataMgr::getInstance()->getTextData()[ch].c_str());
}

void UpgradeCarScene::refreshCost()
{
    //COST
    auto carid = DataMgr::carIndex + 1;
    auto level = 0;
    switch (m_currType)
    {
    case e_type1:
        level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part1_level - 1;
        if ((level + 1) >= DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part1_maxlevel)
        {
            setLabelText(m_bitmapLabelCost, DataMgr::getInstance()->getTextData()["max_level"].c_str());
            m_buttonUpgrade->setEnabled(false);
            m_buttonUpgrade->setBright(false);
        }
        else
        {
            m_upgradeCost = DataMgr::getInstance()->getPartCostData()[carid][level].part1_upgrade_cost;
            setLabelText(m_bitmapLabelCost, m_upgradeCost);
            m_buttonUpgrade->setEnabled(true);
            m_buttonUpgrade->setBright(true);
        }
        break;
    case e_type2:
        level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part2_level - 1;
        if ((level + 1) >= DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part2_maxlevel)
        {
            setLabelText(m_bitmapLabelCost, DataMgr::getInstance()->getTextData()["max_level"].c_str());
            m_buttonUpgrade->setEnabled(false);
            m_buttonUpgrade->setBright(false);
        }
        else
        {
            m_upgradeCost = DataMgr::getInstance()->getPartCostData()[carid][level].part2_upgrade_cost;
            setLabelText(m_bitmapLabelCost, m_upgradeCost);
            m_buttonUpgrade->setEnabled(true);
            m_buttonUpgrade->setBright(true);
        }
        break;
    case e_type3:
        level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part3_level - 1;
        if ((level + 1) >= DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part3_maxlevel)
        {
            setLabelText(m_bitmapLabelCost, DataMgr::getInstance()->getTextData()["max_level"].c_str());
            m_buttonUpgrade->setEnabled(false);
            m_buttonUpgrade->setBright(false);
        }
        else
        {
            m_upgradeCost = DataMgr::getInstance()->getPartCostData()[carid][level].part3_upgrade_cost;
            setLabelText(m_bitmapLabelCost, m_upgradeCost);
            m_buttonUpgrade->setEnabled(true);
            m_buttonUpgrade->setBright(true);
        }
        break;
    case e_type4:
        level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part4_level - 1;
        if ((level + 1) >= DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part4_maxlevel)
        {
            setLabelText(m_bitmapLabelCost, DataMgr::getInstance()->getTextData()["max_level"].c_str());
            m_buttonUpgrade->setEnabled(false);
            m_buttonUpgrade->setBright(false);
        }
        else
        {
            m_upgradeCost = DataMgr::getInstance()->getPartCostData()[carid][level].part4_upgrade_cost;
            setLabelText(m_bitmapLabelCost, m_upgradeCost);
            m_buttonUpgrade->setEnabled(true);
            m_buttonUpgrade->setBright(true);
        }
        break;
    default:
        break;
    }
}

void UpgradeCarScene::refreshLevel()
{
    //LEVEL
    char ch[32];
    sprintf(ch, "BitmapLabel_Level%d", m_currType + 1);
    auto levelPart = Helper::seekWidgetByName(m_view, ch);
    auto levelText = DataMgr::getInstance()->getTextData()["upgrade_level"];
    sprintf(ch, levelText.c_str(), m_partData[m_currType].level, m_partData[m_currType].maxlevel);
    setLabelText(levelPart, ch);
}

void UpgradeCarScene::upgradeCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            AudioEnginMgr::getInstance()->playBtnEffect();
            if (UtilHelper::getFromInteger(USER_GOLD) >= m_upgradeCost)
            {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
                umeng::MobClickCpp::event("user_upgrade", String::createWithFormat("%d",m_currType)->_string.c_str());
#endif
                CarModType type;
                auto level = 1;
                auto gold = 0;
                auto carid = DataMgr::carIndex + 1;
                switch (m_currType)
                {
                case e_type1:
                    {
                        type = e_part1_level;
                        level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part1_level + 1;
                        if (level > DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part1_maxlevel)
                        {
                            level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part1_maxlevel;
                        }
                        gold = UtilHelper::getFromInteger(USER_GOLD) - DataMgr::getInstance()->getPartCostData()[carid][level - 2].part1_upgrade_cost;
                    }
                    break;
                case e_type2:
                    {
                        type = e_part2_level;
                        level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part2_level + 1;
                        if (level > DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part2_maxlevel)
                        {
                            level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part2_maxlevel;
                        }
                        gold = UtilHelper::getFromInteger(USER_GOLD) - DataMgr::getInstance()->getPartCostData()[carid][level - 2].part2_upgrade_cost;
                    }
                    break;
                case e_type3:
                    {
                        type = e_part3_level;
                        level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part3_level + 1;
                        if (level > DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part3_maxlevel)
                        {
                            level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part3_maxlevel;
                        }
                        gold = UtilHelper::getFromInteger(USER_GOLD) - DataMgr::getInstance()->getPartCostData()[carid][level - 2].part3_upgrade_cost;
                    }
                    break;
                case e_type4:
                    {
                        type = e_part4_level;
                        level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part4_level + 1;
                        if (level > DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part4_maxlevel)
                        {
                            level = DataMgr::getInstance()->getCarData()[DataMgr::carIndex].part4_maxlevel;
                        }
                        gold = UtilHelper::getFromInteger(USER_GOLD) - DataMgr::getInstance()->getPartCostData()[carid][level - 2].part4_upgrade_cost;
                    }
                    break;
                default:
                    break;
                }
                if (UtilHelper::writeCarData(carid, type, level))
                {
                    DataMgr::getInstance()->setCarData(carid - 1, type, level);
                    initPartData();
                    refreshCost();
                    refreshLevel();
                    setLabelText(m_labelGold, gold);
                    UtilHelper::writeToInteger(USER_GOLD, gold);
                }
            }
            else
            {
                ConfirmDialog* dlg = new ConfirmDialog();
                dlg->setEnough(false);
                char ch[128];
                sprintf(ch, "%s\n%s", 
                    DataMgr::getInstance()->getTextData()["lack_coins"].c_str(), 
                    DataMgr::getInstance()->getTextData()["get_more"].c_str());
                dlg->setInfo(ch);
                //dlg->setCallbackFunc(this, callfunc_selector(SelectMapScene::refreshUI));
                dlg->init();
                this->addChild(dlg, 999);
                dlg->release();
            }
        }
        break;
    default:
        break;
    }
}

void UpgradeCarScene::backCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            AudioEnginMgr::getInstance()->playBtnEffect();
            Director::getInstance()->replaceScene(TransitionSlideInL::create(0.5f, SelectCarScene::createScene()));
        }
        break;
    default:
        break;
    }
}

void UpgradeCarScene::goCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            AudioEnginMgr::getInstance()->playBtnEffect();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GameScene::createScene()));
        }
        break;
    default:
        break;
    }
}

void UpgradeCarScene::clickCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_BEGAN:
        {
            for (int i = 0; i < e_type_count; i++)
            {
                if (sender == m_upgradeTypes[i])
                {
                    if (m_currType != (UPGRADETYPE)i)
                    {
                        m_currType = (UPGRADETYPE)i;
                        m_upgradeTypes[m_currType]->setBright(false);
                        refreshDesc();
                        refreshCost();
                    }
                }
                else
                {
                    m_upgradeTypes[i]->setBright(true);
                }
            }
        }
        break;
    default:
        break;
    }
}
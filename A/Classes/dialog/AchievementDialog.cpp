#include "AchievementDialog.h"
#include "UtilHelper.h"
#include "DataMgr.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "MobClickCpp.h"
#endif

Scene* AchievementDialog::createScene()
{
    auto scene = Scene::create();
    auto layer = AchievementDialog::create();
    scene->addChild(layer);
    return scene;
}

AchievementDialog::AchievementDialog()
{
    m_pAchievementListView = NULL;
    m_iListIndex = 0;
}

AchievementDialog::~AchievementDialog()
{
}

bool AchievementDialog::init()
{
    if ( !Base::init() )
    {
        return false;
    }
    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/AchievementDialog.ExportJson");
    m_view->setSize(getVisableSize());
    m_view->setPosition(VisibleRect::leftBottom());
    this->addChild(m_view);

    initUI();
    
    return true;
}
void AchievementDialog::initUI()
{
    auto resetTime = Helper::seekWidgetByName(m_view, "BitmapLabel_Time");
    setLabelText(resetTime, UtilHelper::getResetTimes());

    auto imageBack = Helper::seekWidgetByName(m_view, "Image_Back");
    imageBack->addTouchEventListener(this, toucheventselector(AchievementDialog::backCallback));
    

    m_pAchievementListView = dynamic_cast<ListView*>(Helper::seekWidgetByName(m_view, "ListView_Achievement"));
    m_pAchievementListView->addEventListenerListView(this, listvieweventselector(AchievementDialog::selectedItemEvent));
    
    vector<Achievement> achievements =  UtilHelper::readAchievementData();

    for (int i = 0; i < achievements.size(); i++)
    {
        auto achievementItem = GUIReader::getInstance()->widgetFromJsonFile("UI/TaskListItem.ExportJson");

        auto bitmapLabelDesc = Helper::seekWidgetByName(achievementItem, "Label_TaskDesc");
        auto bitmapLabelProgress = Helper::seekWidgetByName(achievementItem, "Label_Progress");
        string desc = "";
        string progress = "0";
        bool isComplete = false;
        
        if (achievements[i].type == 0)
        {
            desc = " complete " + String::createWithFormat("%d",achievements[i].target)->_string +" races";
           
            if (UtilHelper::getFromInteger(OVER_COUNT) >= achievements[i].target)
            {
                isComplete = true;
            }
            else
            {
                isComplete = false;
            }
            progress = String::createWithFormat("%d", UtilHelper::getFromInteger(OVER_COUNT))->_string;
            
        }
        else
        {
            desc = " reach " + String::createWithFormat("%d",achievements[i].target)->_string +"m";
            if (UtilHelper::getFromInteger(TODAY_DISTANCE) >= achievements[i].target)
            {
                isComplete = true;
            }
            else
            {
                isComplete = false;
            }
            progress = String::createWithFormat("%d", UtilHelper::getFromInteger(TODAY_DISTANCE))->_string;
        }

        progress += "/" + String::createWithFormat("%d",achievements[i].target)->_string;
        setLabelText(bitmapLabelDesc, desc);
        setLabelText(bitmapLabelProgress, progress);
        
        auto bitmapLabelCoins = Helper::seekWidgetByName(achievementItem, "BitmapLabel_Coins");
        setLabelText(bitmapLabelCoins, String::createWithFormat("%d",achievements[i].conis)->_string );


        auto btnGetReward = Helper::seekWidgetByName(achievementItem, "Button_GetReward");
        btnGetReward->addTouchEventListener(this, toucheventselector(AchievementDialog::getRewardCallback));
        // 没有领取 但是完成了任务
        if (achievements[i].receive == 0 && isComplete)
        {
            btnGetReward->setVisible(true);
        }
        else
        {
            btnGetReward->setVisible(false);
        }

        auto imageDone = Helper::seekWidgetByName(achievementItem, "Image_Done");
        // 领取了任务
        if (achievements[i].receive == 1)
        {
            imageDone->setVisible(true);
        }

        m_pAchievementListView->pushBackCustomItem(achievementItem);

    }
}

void AchievementDialog::getRewardCallback(Ref* sender,TouchEventType type)
{
     switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            if (UtilHelper::writeAchievementData(m_iListIndex))
            {
            
                auto achievementItem = m_pAchievementListView->getItem(m_iListIndex);
                auto btnGetReward = Helper::seekWidgetByName(achievementItem, "Button_GetReward");
                btnGetReward->setVisible(false);
                auto imageDone = Helper::seekWidgetByName(achievementItem, "Image_Done");
                imageDone->setVisible(true);
                UtilHelper::writeToBool(WRITE_DATA, true);

                vector<Achievement> achievements =  UtilHelper::readAchievementData();
                int nTotalCoins = UtilHelper::getFromInteger(USER_GOLD) + achievements[m_iListIndex].conis; 
                UtilHelper::writeToInteger(USER_GOLD, nTotalCoins);
                dynamic_cast<Base*>(this->getParent())->refreshCoins();
            }
        }
        break;
    default:
        break;
    }
}

void AchievementDialog::backCallback(Ref* sender,TouchEventType type)
{
     switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            remove();
        }
        break;
    default:
        break;
    }
}

void AchievementDialog::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
    switch (type)
    {
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
        {
            ListView* listView = static_cast<ListView*>(pSender);
            CC_UNUSED_PARAM(listView);
            CCLOG("select child start index = %ld", listView->getCurSelectedIndex());
            m_iListIndex = listView->getCurSelectedIndex();
            break;
        }
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
        {
            ListView* listView = static_cast<ListView*>(pSender);
            CC_UNUSED_PARAM(listView);
            CCLOG("select child end index = %ld", listView->getCurSelectedIndex());
            
            break;
        }
        default:
            break;
    }
}
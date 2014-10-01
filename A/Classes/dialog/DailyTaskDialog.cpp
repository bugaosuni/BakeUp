#include "DailyTaskDialog.h"
#include "UtilHelper.h"
#include "DataMgr.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "MobClickCpp.h"
#endif

Scene* DailyTaskDialog::createScene()
{
    auto scene = Scene::create();
    auto layer = DailyTaskDialog::create();
    scene->addChild(layer);
    return scene;
}

DailyTaskDialog::DailyTaskDialog()
{
    m_pDailyTaskListView = NULL;
    m_iListIndex = 0;
}

DailyTaskDialog::~DailyTaskDialog()
{
}

bool DailyTaskDialog::init()
{
    if ( !Base::init() )
    {
        return false;
    }
    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/DailyTaskDialog.ExportJson");
    m_view->setSize(getVisableSize());
    m_view->setPosition(VisibleRect::leftBottom());
    this->addChild(m_view);

    initUI();
    
    return true;
}
void DailyTaskDialog::initUI()
{
    auto resetTime = Helper::seekWidgetByName(m_view, "BitmapLabel_Time");
    setLabelText(resetTime, UtilHelper::getResetTimes());

    auto imageBack = Helper::seekWidgetByName(m_view, "Image_Back");
    imageBack->addTouchEventListener(this, toucheventselector(DailyTaskDialog::backCallback));
    

    m_pDailyTaskListView = dynamic_cast<ListView*>(Helper::seekWidgetByName(m_view, "ListView_DailyTask"));
    m_pDailyTaskListView->addEventListenerListView(this, listvieweventselector(DailyTaskDialog::selectedItemEvent));
    
    vector<DailyTask> dailyTasks =  UtilHelper::readDailyTaskData();

    for (int i = 0; i < dailyTasks.size(); i++)
    {
        auto dailyTaskItem = GUIReader::getInstance()->widgetFromJsonFile("UI/TaskListItem.ExportJson");

        auto bitmapLabelDesc = Helper::seekWidgetByName(dailyTaskItem, "Label_TaskDesc");
        auto bitmapLabelProgress = Helper::seekWidgetByName(dailyTaskItem, "Label_Progress");
        string desc = "";
        string progress = "0";
        bool isComplete = false;
        
        if (dailyTasks[i].type == 0)
        {
            desc = " complete " + String::createWithFormat("%d",dailyTasks[i].target)->_string +" races";
           
            if (UtilHelper::getFromInteger(TODAY_COUNT) >= dailyTasks[i].target)
            {
                isComplete = true;
            }
            else
            {
                isComplete = false;
            }
            progress = String::createWithFormat("%d", UtilHelper::getFromInteger(TODAY_COUNT))->_string;
            
        }
        else
        {
            desc = " reach " + String::createWithFormat("%d",dailyTasks[i].target)->_string +"m";
            if (UtilHelper::getFromInteger(TODAY_DISTANCE) >= dailyTasks[i].target)
            {
                isComplete = true;
            }
            else
            {
                isComplete = false;
            }
            progress = String::createWithFormat("%d", UtilHelper::getFromInteger(TODAY_DISTANCE))->_string;
        }

        progress += "/" + String::createWithFormat("%d",dailyTasks[i].target)->_string;
        setLabelText(bitmapLabelDesc, desc);
        setLabelText(bitmapLabelProgress, progress);
        
        auto bitmapLabelCoins = Helper::seekWidgetByName(dailyTaskItem, "BitmapLabel_Coins");
        setLabelText(bitmapLabelCoins, String::createWithFormat("%d",dailyTasks[i].conis)->_string );


        auto btnGetReward = Helper::seekWidgetByName(dailyTaskItem, "Button_GetReward");
        btnGetReward->addTouchEventListener(this, toucheventselector(DailyTaskDialog::getRewardCallback));
        // 没有领取 但是完成了任务
        if (dailyTasks[i].receive == 0 && isComplete)
        {
            btnGetReward->setVisible(true);
        }
        else
        {
            btnGetReward->setVisible(false);
        }

        auto imageDone = Helper::seekWidgetByName(dailyTaskItem, "Image_Done");
        // 领取了任务
        if (dailyTasks[i].receive == 1)
        {
            imageDone->setVisible(true);
        }

        m_pDailyTaskListView->pushBackCustomItem(dailyTaskItem);

    }
}

void DailyTaskDialog::getRewardCallback(Ref* sender,TouchEventType type)
{
     switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            if (UtilHelper::writeDailyTaskData(m_iListIndex))
            {
            
                auto dailyTaskItem = m_pDailyTaskListView->getItem(m_iListIndex);
                auto btnGetReward = Helper::seekWidgetByName(dailyTaskItem, "Button_GetReward");
                btnGetReward->setVisible(false);
                auto imageDone = Helper::seekWidgetByName(dailyTaskItem, "Image_Done");
                imageDone->setVisible(true);
                UtilHelper::writeToBool(WRITE_DATA, true);

                

                vector<DailyTask> dailyTasks =  UtilHelper::readDailyTaskData();
                int nTotalCoins = UtilHelper::getFromInteger(USER_GOLD) + dailyTasks[m_iListIndex].conis; 
                UtilHelper::writeToInteger(USER_GOLD, nTotalCoins);
                dynamic_cast<Base*>(this->getParent())->refreshCoins();
            }
        }
        break;
    default:
        break;
    }
}

void DailyTaskDialog::backCallback(Ref* sender,TouchEventType type)
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

void DailyTaskDialog::selectedItemEvent(Ref *pSender, ListView::EventType type)
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
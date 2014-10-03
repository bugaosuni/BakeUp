#include "SelectMapScene.h"
#include "SelectCarScene.h"
#include "ConfirmDialog.h"
#include "UtilHelper.h"
#include "DataMgr.h"
#include "DailyTaskDialog.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include "MobClickCpp.h"
#endif

#define CELLWIDTH (340.0f)
#define FACTOR (350.0f)
#define MASK (150.0f)

Scene* SelectMapScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SelectMapScene::create();
    scene->addChild(layer);
    return scene;
}

SelectMapScene::SelectMapScene()
{
    m_labelGold = nullptr;
    m_listView = nullptr;
    m_scrolling = false;
    m_selectedIndex = 1;
}

SelectMapScene::~SelectMapScene()
{
}

bool SelectMapScene::init()
{
    if ( !Base::init() )
    {
        return false;
    }

    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/SelectMapScene.ExportJson");
    m_view->setSize(getVisableSize());
    m_view->setPosition(VisibleRect::leftBottom());
    this->addChild(m_view);

    initUI();
    initListView();

    m_listener->onKeyReleased = CC_CALLBACK_2(SelectMapScene::onKeyReleased, this); 
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener, this); 
    
    return true;
}

void SelectMapScene::initUI()
{
    //title
    auto bitmapLabelTitle = Helper::seekWidgetByName(m_view, "BitmapLabel_Title");
    setLabelText(bitmapLabelTitle, DataMgr::getInstance()->getTextData()["map_title"].c_str());

    //label gold
    m_labelGold = Helper::seekWidgetByName(m_view, "BitmapLabel_Gold");
    setLabelText(m_labelGold, UtilHelper::getFromInteger(USER_GOLD));

    //daily task
    auto buttonAdd = Helper::seekWidgetByName(m_view, "Button_DailyTask");
    buttonAdd->addTouchEventListener(this, toucheventselector(SelectMapScene::dailyTaskCallback));

    auto achievementTask = Helper::seekWidgetByName(m_view, "Button_Achievement");
    achievementTask->addTouchEventListener(this, toucheventselector(SelectMapScene::achievementCallback));

    //settings
    auto buttonSettings = Helper::seekWidgetByName(m_view, "Button_Settings");
    buttonSettings->addTouchEventListener(this, toucheventselector(SelectMapScene::settingsCallback));

    //back
    auto buttonBack = Helper::seekWidgetByName(m_view, "Button_Back");
    buttonBack->setVisible(false);
   // buttonBack->addTouchEventListener(this, toucheventselector(SelectMapScene::backCallback));

    //go
    auto buttonGo = Helper::seekWidgetByName(m_view, "Button_Go");
    buttonGo->addTouchEventListener(this, toucheventselector(SelectMapScene::goCallback));

    addOwnAds();

    if(!DataMgr::bIsShowDailyTask)
    {
        DataMgr::bIsShowDailyTask = true;
        DailyTaskDialog* dlg = new DailyTaskDialog();
        dlg->init();
        this->addChild(dlg, 999);
        dlg->release();
    }
}

void SelectMapScene::initListView()
{
    //ListView
    m_listView = dynamic_cast<ListView*>(Helper::seekWidgetByName(m_view, "ListView_Map"));
    m_listView->addEventListenerScrollView(this, scrollvieweventselector(SelectMapScene::scrollViewCallBack));
    m_listView->addEventListenerListView(this, listvieweventselector(SelectMapScene::selectedItemEvent));

    auto mapSize = DataMgr::getInstance()->getMapData().size();
    for (int i = 0; i < mapSize + 2; i++)
    {
        auto cell = GUIReader::getInstance()->widgetFromJsonFile("UI/ListItem.ExportJson");
        auto mask1 = Helper::seekWidgetByName(cell, "Image_Mask");
        mask1->setOpacity(0.0f);
        if (i == 0 || i == mapSize + 1)
        {
            cell->setVisible(false);
            m_listView->pushBackCustomItem(cell);
            continue;
        }
        if (i == 1)
        {
            cell->setScale(sqrt(510 / FACTOR));
        }
        else
        {
            cell->setScale(sqrt(170 / FACTOR));
            mask1->setOpacity(MASK);
        }
        //name
        auto name = DataMgr::getInstance()->getMapData()[i-1].name;
        auto bitmapLabelName = Helper::seekWidgetByName(cell, "BitmapLabel_Name");
        setLabelText(bitmapLabelName, DataMgr::getInstance()->getTextData()[name].c_str());
        //image
        auto image = DataMgr::getInstance()->getMapData()[i-1].image;
        Sprite* sp = Sprite::createWithSpriteFrameName(image);

        if (sp != NULL)
        {
            sp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            sp->setPosition(Vec2::ZERO);
            cell->addChild(sp);
        }
        //open info
        auto panelMask = Helper::seekWidgetByName(cell, "Panel_Lock");
        auto bitmapLabelInfo = Helper::seekWidgetByName(cell, "BitmapLabel_Info");
        auto open = DataMgr::getInstance()->getMapData()[i-1].open;
        char ch[32];
        if (open == 1)
        {
            panelMask->setVisible(false);
            sprintf(ch, DataMgr::getInstance()->getTextData()["map_best"].c_str(), DataMgr::getInstance()->getMapData()[i-1].bestDist);
            setLabelText(bitmapLabelInfo, ch);
        }
        else
        {
            panelMask->setVisible(true);
            sprintf(ch, DataMgr::getInstance()->getTextData()["cost"].c_str(), DataMgr::getInstance()->getMapData()[i-1].cost);
            setLabelText(bitmapLabelInfo, ch);
        }
        m_listView->pushBackCustomItem(cell);
    }
}

void SelectMapScene::scrollListView(float offsetX)
{
    //for ѭ�����������е�ÿ������  
    for( auto e : m_listView->getItems() )  
    {
        auto mask1 = Helper::seekWidgetByName(e, "Image_Mask");
        auto pointX = e->getPositionX();//��õ�ǰ�����X��꣨������ô�����������궼�ǲ���ģ�  
        float endPosX = pointX + offsetX;//������� X��� + ƫ��X���

        if( endPosX > 170 && endPosX <= 510 )  
        {  
            float x = sqrt(endPosX / FACTOR);
            e->setScale(x);
            mask1->setOpacity(MASK * (510 / FACTOR - x));
        }  
        else if( endPosX > 510 && endPosX < 850 )   
        {  
            //���������ʽ���ý��ͣ��Ҿ���ô˵�ɣ�  
            //���� endPosX = 200����ô�Ŵ���Ӧ���� 200 / 150 = 1.33���ң���ô��endPosX = 300ʱ�����ڶԳƵ�ԭ�?������512Ϊ�Գ����ģ���ô
            //300 �ķŴ���ҲӦ���� 1.33�����������Ĺ�ʽ����  
            float a = endPosX - 510;  
            float b = 510 - a;  

            float x = sqrt(b / FACTOR);
            e->setScale(x); 
            mask1->setOpacity(MASK * (510 / FACTOR - x));
        }  
        else  
        {  
            //����������ķ�Χ��������Ϊ���С  
            //e->setScale(1.0f);
        }  
    } 
}

void SelectMapScene::goCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        AudioEnginMgr::getInstance()->playBtnEffect();
        selectEvent();
        break;
    default:
        break;
    }
}

void SelectMapScene::scrollViewCallBack(Ref* sender, ScrollviewEventType type)
{
    m_scrolling = true;
    ListView* view = dynamic_cast<ListView*>(sender);
    //��scrollView�϶�ʱ��Ӧ�ú���  
    auto offsetPosX = view->getInnerContainer()->getPositionX();//���ƫ��X���(�����ƶ���ƫ����Ϊ����������Ϊ����  

    scrollListView(offsetPosX);
}

void SelectMapScene::selectedItemEvent(Ref* sender, ListViewEventType type)
{
    ListView* view = dynamic_cast<ListView*>(sender);
    ssize_t index = view->getCurSelectedIndex();
    auto count = (view->getInnerContainerSize().width - view->getSize().width) / CELLWIDTH;

    switch (type)
    {
    case LISTVIEW_ONSELECTEDITEM_START:
        {
            m_scrolling = false;
        }
        break;
    case LISTVIEW_ONSELECTEDITEM_END:
        {
            if (!m_scrolling)
            {
                if (index == m_selectedIndex)//select current index
                {
                    selectEvent();
                }
                else if ((index < m_selectedIndex && index > 0) || (index > m_selectedIndex && index < view->getItems().size() - 1)) 
                {
                    //go to pre or next page
                    m_selectedIndex = index;
                    view->scrollToPercentHorizontal(100 / count * (index - 1), 0.3f, false);
                }
            }
            else
            {
                this->unscheduleAllSelectors();
                this->stopAllActions();
                auto offsetPosX = view->getInnerContainer()->getPositionX();
                int dist = (int)(offsetPosX / -CELLWIDTH + 0.5f);
                m_selectedIndex = dist + 1;
                view->scrollToPercentHorizontal(100 / count * dist, 0.3f, false);
            }
        }
        break;
    default:
        break;
    }
}

void SelectMapScene::selectEvent()
{
    DataMgr::mapIndex = m_selectedIndex - 1;
    if (DataMgr::getInstance()->getMapData()[DataMgr::mapIndex].open == 1)
    {
        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f, SelectCarScene::createScene()));
    }
    else
    {
        auto enough = UtilHelper::getFromInteger(USER_GOLD) >= DataMgr::getInstance()->getMapData()[DataMgr::mapIndex].cost;
        ConfirmDialog* dlg = new ConfirmDialog();
        dlg->setEnough(enough);
        char ch[128];
        if (enough)
        {
            char ch1[32];
            sprintf(ch1, DataMgr::getInstance()->getTextData()["cost"].c_str(), DataMgr::getInstance()->getMapData()[DataMgr::mapIndex].cost);
            sprintf(ch, "%s\n%s", 
                DataMgr::getInstance()->getTextData()["unlock_tip"].c_str(), 
                ch1);
        }
        else
        {
            sprintf(ch, "%s\n%s", 
                DataMgr::getInstance()->getTextData()["lack_coins"].c_str(), 
                DataMgr::getInstance()->getTextData()["get_more"].c_str());
        }
        dlg->setInfo(ch);
        dlg->setCallbackFunc(this, callfunc_selector(SelectMapScene::refreshUI));
        dlg->init();
        this->addChild(dlg, 999);
        dlg->release();
    }
}

void SelectMapScene::refreshUI()
{
    auto index = DataMgr::mapIndex;
    if (UtilHelper::writeMapData(index + 1, e_map_open, 1))
    {
        auto cell = m_listView->getItem(index + 1);
        //open info
        auto panelMask = Helper::seekWidgetByName(cell, "Panel_Lock");
        auto bitmapLabelInfo = Helper::seekWidgetByName(cell, "BitmapLabel_Info");
        char ch[32];
        panelMask->setVisible(false);
        sprintf(ch, DataMgr::getInstance()->getTextData()["map_best"].c_str(), DataMgr::getInstance()->getMapData()[index].bestDist);
        setLabelText(bitmapLabelInfo, ch);

        DataMgr::getInstance()->setMapData(index, e_map_open, 1);
        auto gold = UtilHelper::getFromInteger(USER_GOLD) - DataMgr::getInstance()->getMapData()[index].cost;
        setLabelText(m_labelGold, gold);
        UtilHelper::writeToInteger(USER_GOLD, gold);
    }
}

void SelectMapScene::onKeyReleased(EventKeyboard::KeyCode keycode,Event * pEvent) 
{ 
    if(EventKeyboard::KeyCode::KEY_RETURN == keycode || EventKeyboard::KeyCode::KEY_ESCAPE == keycode || EventKeyboard::KeyCode::KEY_BACKSPACE == keycode)
    {
        if (!DataMgr::modalShow)
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            umeng::MobClickCpp::event("user_exit");
            JniMethodInfo t;
            JniHelper::getStaticMethodInfo(t, "com/game/infinite/racing/Airpush", "airSmartWallAd", "()V");
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
#endif
            UtilHelper::showStartAppAd(2);
        }
    }
}

#include "SelectMapScene.h"
#include "SelectCarScene.h"
#include "ConfirmDialog.h"
#include "UpgradeCarScene.h"
#include "UtilHelper.h"
#include "DataMgr.h"

#define CELLWIDTH (340.0f)
#define FACTOR (350.0f)
#define MASK (100.0f)

Scene* SelectCarScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SelectCarScene::create();
    scene->addChild(layer);
    return scene;
}

SelectCarScene::SelectCarScene()
{
    m_labelGold = nullptr;
    m_listView = nullptr;
    m_scrolling = false;
    m_selectedIndex = 1;
}

SelectCarScene::~SelectCarScene()
{
}

bool SelectCarScene::init()
{
    if ( !Base::init() )
    {
        return false;
    }

    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/SelectCarScene.ExportJson");
    this->addChild(m_view);

    initUI();
    initListView();

    m_listener->onKeyReleased = CC_CALLBACK_2(SelectCarScene::onKeyReleased, this); 
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener, this); 

    return true;
}

void SelectCarScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    if(EventKeyboard::KeyCode::KEY_RETURN == keycode || EventKeyboard::KeyCode::KEY_ESCAPE == keycode || EventKeyboard::KeyCode::KEY_BACKSPACE == keycode)
    {
        if (!DataMgr::modalShow)
        {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectMapScene::createScene()));
        }
    }
}

void SelectCarScene::initUI()
{
    auto panelRoot = Helper::seekWidgetByName(m_view, "Panel_Root");
    panelRoot->setSize(getVisableSize());
    panelRoot->setPosition(VisibleRect::leftBottom());

    //title
    auto bitmapLabelTitle = Helper::seekWidgetByName(m_view, "BitmapLabel_Title");
    setLabelText(bitmapLabelTitle, DataMgr::getInstance()->getTextData()["car_title"].c_str());

    //label gold
    m_labelGold = Helper::seekWidgetByName(m_view, "BitmapLabel_Gold");
    setLabelText(m_labelGold, UtilHelper::getFromInteger(USER_GOLD));

    //add
    auto buttonAdd = Helper::seekWidgetByName(m_view, "Button_Add");
    buttonAdd->addTouchEventListener(this, toucheventselector(SelectCarScene::addCallback));

    //settings
    auto buttonSettings = Helper::seekWidgetByName(m_view, "Button_Settings");
    buttonSettings->addTouchEventListener(this, toucheventselector(SelectCarScene::settingsCallback));

    //back
    auto buttonBack = Helper::seekWidgetByName(m_view, "Button_Back");
    buttonBack->addTouchEventListener(this, toucheventselector(SelectCarScene::backCallback));

    //go
    auto buttonGo = Helper::seekWidgetByName(m_view, "Button_Go");
    buttonGo->addTouchEventListener(this, toucheventselector(SelectCarScene::goCallback));

	addOwnAds();
}


void SelectCarScene::initListView()
{
    //ListView
    m_listView = dynamic_cast<ListView*>(Helper::seekWidgetByName(m_view, "ListView_Car"));
    m_listView->addEventListenerScrollView(this, scrollvieweventselector(SelectCarScene::scrollViewCallBack));
    m_listView->addEventListenerListView(this, listvieweventselector(SelectCarScene::selectedItemEvent));

    auto carSize = DataMgr::getInstance()->getCarData().size();
    for (int i = 0; i < carSize + 2; i++)
    {
        auto cell = GUIReader::getInstance()->widgetFromJsonFile("UI/CellScene.ExportJson");
        auto mask1 = Helper::seekWidgetByName(cell, "Image_Mask");
        mask1->setOpacity(0.0f);
        if (i == 0 || i == carSize + 1)
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
        auto name = DataMgr::getInstance()->getCarData()[i-1].name;
        auto bitmapLabelName = Helper::seekWidgetByName(cell, "BitmapLabel_Name");
        setLabelText(bitmapLabelName, DataMgr::getInstance()->getTextData()[name].c_str());
        //image
        auto image = DataMgr::getInstance()->getCarData()[i-1].image;
        Sprite* sp = Sprite::createWithSpriteFrameName(image);

        if (sp != NULL)
        {
            sp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            sp->setPosition(Vec2::ZERO);
            cell->addChild(sp);
        }
        //open info
        auto panelMask = Helper::seekWidgetByName(cell, "Panel_Mask");
        auto bitmapLabelInfo = Helper::seekWidgetByName(cell, "BitmapLabel_Info");
        auto open = DataMgr::getInstance()->getCarData()[i-1].open;
        char ch[32];
        if (open == 1)
        {
            panelMask->setVisible(false);
            setLabelText(bitmapLabelInfo, "");
        }
        else
        {
            panelMask->setVisible(true);
            sprintf(ch, DataMgr::getInstance()->getTextData()["cost"].c_str(), DataMgr::getInstance()->getCarData()[i-1].cost);
            setLabelText(bitmapLabelInfo, ch);
        }
        m_listView->pushBackCustomItem(cell);
    }
}


void SelectCarScene::scrollListView(float offsetX)
{
    //for 循环遍历容器中的每个精灵  
    for( auto e : m_listView->getItems() )  
    {
        auto mask1 = Helper::seekWidgetByName(e, "Image_Mask");
        auto pointX = e->getPositionX();//获得当前对象的X坐标（不管怎么滚动，这个坐标都是不变的）  
        float endPosX = pointX + offsetX;//将精灵的 X坐标 + 偏移X坐标

        if( endPosX > 170 && endPosX <= 510 )  
        {  
            float x = sqrt(endPosX / FACTOR);
            e->setScale(x);
            mask1->setOpacity(MASK * (510 / FACTOR - x));
        }  
        else if( endPosX > 510 && endPosX < 850 )   
        {  
            //下面这个公式不好解释，我就这么说吧：  
            //假设 endPosX = 200，那么放大倍数应该是 200 / 150 = 1.33左右，那么当endPosX = 300时，出于对称的原理，我们以512为对称中心，那么  
            //300 的放大倍数也应该是 1.33。这就是下面的公式由来  
            float a = endPosX - 510;  
            float b = 510 - a;  

            float x = sqrt(b / FACTOR);
            e->setScale(x); 
            mask1->setOpacity(MASK * (510 / FACTOR - x));
        }  
        else  
        {  
            //不是在上面的范围，则设置为正常大小  
            //e->setScale(1.0f);
        }  
    } 
}

void SelectCarScene::backCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            AudioEnginMgr::getInstance()->playBtnEffect();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectMapScene::createScene()));
        }
        break;
    default:
        break;
    }
}

void SelectCarScene::goCallback(Ref* sender,TouchEventType type)
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

void SelectCarScene::scrollViewCallBack(Ref* sender, ScrollviewEventType type)
{
    m_scrolling = true;
    ListView* view = dynamic_cast<ListView*>(sender);
    //在scrollView拖动时响应该函数 
    auto offsetPosX = view->getInnerContainer()->getPositionX();//获得偏移X坐标(向右移动，偏移量为正数，向左则为负数）  

    scrollListView(offsetPosX);
}

void SelectCarScene::selectedItemEvent(Ref* sender, ListViewEventType type)
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


void SelectCarScene::selectEvent()
{
    DataMgr::carIndex = m_selectedIndex - 1;
    if (DataMgr::getInstance()->getCarData()[DataMgr::carIndex].open == 1)
    {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, UpgradeCarScene::createScene()));
    }
    else
    {
        auto enough = UtilHelper::getFromInteger(USER_GOLD) >= DataMgr::getInstance()->getCarData()[DataMgr::carIndex].cost;
        ConfirmDialog* dlg = new ConfirmDialog();
        dlg->setEnough(enough);
        char ch[128];
        if (enough)
        {
            char ch1[32];
            sprintf(ch1, DataMgr::getInstance()->getTextData()["cost"].c_str(), DataMgr::getInstance()->getCarData()[DataMgr::carIndex].cost);
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
        dlg->setCallbackFunc(this, callfunc_selector(SelectCarScene::refreshUI));
        dlg->init();
        this->addChild(dlg, 999);
        dlg->release();
    }
}


void SelectCarScene::refreshUI()
{
    auto index = DataMgr::carIndex;
    if (UtilHelper::writeCarData(index + 1, e_car_open, 1))
    {
        auto cell = m_listView->getItem(index);
        //open info
        auto panelMask = Helper::seekWidgetByName(cell, "Panel_Mask");
        auto bitmapLabelInfo = Helper::seekWidgetByName(cell, "BitmapLabel_Info");
        panelMask->setVisible(false);
        setLabelText(bitmapLabelInfo, "");

        DataMgr::getInstance()->setCarData(index, e_car_open, 1);
        auto gold = UtilHelper::getFromInteger(USER_GOLD) - DataMgr::getInstance()->getCarData()[index].cost;
        setLabelText(m_labelGold, gold);
        UtilHelper::writeToInteger(USER_GOLD, gold);
    }
}

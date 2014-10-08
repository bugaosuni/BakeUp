#include "WorldScene.h"
#include "MainScene.h"
#include "LevelScene.h"
#include "DataMgr.h"

#define CELLWIDTH (340.0f)
#define FACTOR (350.0f)

Scene* WorldScene::createScene()
{
    auto scene = Scene::create();
    auto layer = WorldScene::create();
    scene->addChild(layer);
    return scene;
}

WorldScene::WorldScene()
{
    m_listView = nullptr;
    m_scrolling = false;
    m_selectedIndex = 1;
}

WorldScene::~WorldScene()
{
}

bool WorldScene::init()
{
    if ( !Base::init() )
    {
        return false;
    }

    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/World.ExportJson");
    this->addChild(m_view);

    initUI();
    initListView();

    m_listener->onKeyReleased = CC_CALLBACK_2(WorldScene::onKeyReleased, this); 
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener, this); 
    
    return true;
}

void WorldScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    if(EventKeyboard::KeyCode::KEY_RETURN == keycode || EventKeyboard::KeyCode::KEY_ESCAPE == keycode || EventKeyboard::KeyCode::KEY_BACKSPACE == keycode)
    {
        //if (!DataMgr::modalShow)
        {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MainScene::createScene()));
        }
    }
}

void WorldScene::initUI()
{
    auto panelRoot = Helper::seekWidgetByName(m_view, "Panel_Root");
    panelRoot->setSize(getVisableSize());
    panelRoot->setPosition(VisibleRect::leftBottom());
	Size size = panelRoot->getSize();

	auto actionAllstar = Sequence::createWithTwoActions(DelayTime::create(0.5f),
		EaseExponentialOut::create(MoveTo::create(1.0f, Vec2(0, size.height - 120))));
	auto actionBack = Sequence::createWithTwoActions(DelayTime::create(0.5f),
		EaseExponentialOut::create(MoveTo::create(1.0f, Vec2(60, 60))));
	auto actionBuy = Sequence::createWithTwoActions(DelayTime::create(0.5f),
		EaseExponentialOut::create(MoveTo::create(1.0f, Vec2(size.width - 60, 60))));

    //allstar
    auto panelAllstar = Helper::seekWidgetByName(m_view, "Panel_Allstar");
	auto labelAllstar = Helper::seekWidgetByName(m_view, "Label_Allstar");
	char ch[16];
	sprintf(ch, "%d/%d", DataMgr::getInstance()->getAllCurrStar(), DataMgr::allLevelCount * 3);
    setLabelText(labelAllstar, ch);

    //back
    auto buttonBack = Helper::seekWidgetByName(m_view, "Button_Back");
    buttonBack->addTouchEventListener(this, toucheventselector(WorldScene::backCallback));

    //buy
    auto buttonBuy = Helper::seekWidgetByName(m_view, "Button_Buy");
    buttonBuy->addTouchEventListener(this, toucheventselector(WorldScene::buyCallback));

	panelAllstar->runAction(actionAllstar);
	buttonBack->runAction(actionBack);
	buttonBuy->runAction(actionBuy);

	addOwnAds();
}

void WorldScene::initListView()
{
    //ListView
    m_listView = dynamic_cast<ListView*>(Helper::seekWidgetByName(m_view, "ListView_World"));
    m_listView->addEventListenerScrollView(this, scrollvieweventselector(WorldScene::scrollViewCallBack));
    m_listView->addEventListenerListView(this, listvieweventselector(WorldScene::selectedItemEvent));

    auto worldSize = DataMgr::getInstance()->getWorldData().size();
    for (int i = 0; i < worldSize + 2; i++)
    {
        auto cell = GUIReader::getInstance()->widgetFromJsonFile("UI/WorldCell.ExportJson");
        if (i == 0 || i == worldSize + 1)
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
        }
        //name
        auto name = DataMgr::getInstance()->getWorldData()[i-1].name;
        auto labelName = Helper::seekWidgetByName(cell, "Label_Name");
        setLabelText(labelName, DataMgr::getInstance()->getTextData()[name].c_str());
        //image
        auto image = DataMgr::getInstance()->getWorldData()[i-1].icon;
		ImageView* imageWorld = dynamic_cast<ImageView*>(Helper::seekWidgetByName(cell, "Image_World"));
		imageWorld->loadTexture(image, TextureResType::PLIST);
        
        //open info
        auto imageLock = Helper::seekWidgetByName(cell, "Image_Lock");
        auto panelStar = Helper::seekWidgetByName(cell, "Panel_Star");
        auto open = DataMgr::getInstance()->getWorldData()[i-1].open;
        char ch[32];
        if (open == 1)
        {
            imageLock->setVisible(false);
			panelStar->setVisible(true);
            sprintf(ch, "%d/%d", DataMgr::getInstance()->getWorldStar(i), DataMgr::getInstance()->getLevelData()[i].size() * 3);
			auto labelStar = Helper::seekWidgetByName(cell, "Label_Star");
            setLabelText(labelStar, ch);
        }
        else
        {
            imageLock->setVisible(true);
			panelStar->setVisible(false);
        }
        m_listView->pushBackCustomItem(cell);
    }
}

void WorldScene::scrollListView(float offsetX)
{
    //for 循环遍历容器中的每个精灵  
    for( auto e : m_listView->getItems() )  
    {
        auto pointX = e->getPositionX();//获得当前对象的X坐标（不管怎么滚动，这个坐标都是不变的）  
        float endPosX = pointX + offsetX;//将精灵的 X坐标 + 偏移X坐标

        if( endPosX > 170 && endPosX <= 510 )  
        {  
            float x = sqrt(endPosX / FACTOR);
            e->setScale(x);
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
        }  
        else  
        {  
            //不是在上面的范围，则设置为正常大小  
            //e->setScale(1.0f);
        }  
    } 
}

void WorldScene::backCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
			AudioEnginMgr::playEffect(BACK);
            Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, MainScene::createScene()));
        }
        break;
    default:
        break;
    }
}

void WorldScene::buyCallback(Ref* sender,TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		break;
	default:
		break;
	}
}

void WorldScene::scrollViewCallBack(Ref* sender, ScrollviewEventType type)
{
    m_scrolling = true;
    ListView* view = dynamic_cast<ListView*>(sender);
    //在scrollView拖动时响应该函数  
    auto offsetPosX = view->getInnerContainer()->getPositionX();//获得偏移X坐标(向右移动，偏移量为正数，向左则为负数）  

    scrollListView(offsetPosX);
}

void WorldScene::selectedItemEvent(Ref* sender, ListViewEventType type)
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
					DataMgr::worldId = m_selectedIndex;
					if (DataMgr::getInstance()->getWorldData()[DataMgr::worldId - 1].open == 1)
					{
						AudioEnginMgr::playEffect(BUTTON);
						Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, LevelScene::createScene()));
					}
					else
					{
						AudioEnginMgr::playEffect(LOCKED);
					}
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
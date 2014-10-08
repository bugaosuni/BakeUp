#include "LevelScene.h"
#include "WorldScene.h"
#include "DataMgr.h"
#include "Global.h"
#include "GameScene.h"

Scene* LevelScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LevelScene::create();
    scene->addChild(layer);
    return scene;
}

LevelScene::LevelScene()
{
    m_listView = nullptr;
    m_scrolling = false;
    m_selectedItem = nullptr;
}

LevelScene::~LevelScene()
{
}

bool LevelScene::init()
{
    if ( !Base::init() )
    {
        return false;
    }

    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/Level.ExportJson");
    this->addChild(m_view);

    initUI();
    initListView();

    m_listener->onKeyReleased = CC_CALLBACK_2(LevelScene::onKeyReleased, this); 
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener, this); 
    
    return true;
}

void LevelScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    if(EventKeyboard::KeyCode::KEY_RETURN == keycode || EventKeyboard::KeyCode::KEY_ESCAPE == keycode || EventKeyboard::KeyCode::KEY_BACKSPACE == keycode)
    {
        //if (!DataMgr::modalShow)
        {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, WorldScene::createScene()));
        }
    }
}

void LevelScene::initUI()
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

    //panel gold
    auto panelAllstar = Helper::seekWidgetByName(m_view, "Panel_Allstar");
    auto labelAllstar = Helper::seekWidgetByName(m_view, "Label_Allstar");
    char ch[16];
    sprintf(ch, "%d/%d", DataMgr::getInstance()->getWorldStar(DataMgr::worldId), DataMgr::getInstance()->getLevelData()[DataMgr::worldId].size() * 3);
    setLabelText(labelAllstar, ch);

    //back
    auto buttonBack = Helper::seekWidgetByName(m_view, "Button_Back");
    buttonBack->addTouchEventListener(this, toucheventselector(LevelScene::backCallback));

    //buy
    auto buttonBuy = Helper::seekWidgetByName(m_view, "Button_Buy");
    buttonBuy->addTouchEventListener(this, toucheventselector(LevelScene::buyCallback));

    panelAllstar->runAction(actionAllstar);
    buttonBack->runAction(actionBack);
    buttonBuy->runAction(actionBuy);

    addOwnAds();
}

void LevelScene::initListView()
{
    //ListView
    m_listView = dynamic_cast<ListView*>(Helper::seekWidgetByName(m_view, "ListView_Level"));
    m_listView->addEventListenerScrollView(this, scrollvieweventselector(LevelScene::scrollViewCallBack));
    m_listView->addEventListenerListView(this, listvieweventselector(LevelScene::selectedItemEvent));
    m_listView->setSize(Size(getVisableSize().width, m_listView->getSize().height));

    auto levelSize = DataMgr::getInstance()->getLevelData()[DataMgr::worldId].size();
    for (int i = 0; i < levelSize; i++)
    {
        auto levelData = DataMgr::getInstance()->getLevelData()[DataMgr::worldId][i];
        auto cell = GUIReader::getInstance()->widgetFromJsonFile("UI/LevelCell.ExportJson");
        auto panelLevel = Helper::seekWidgetByName(cell, "Panel_Level");
        panelLevel->setPositionY((i % 2) * cell->getSize().height / 2);
        //image
        auto image = levelData.icon;
        ImageView* imageLevel = dynamic_cast<ImageView*>(Helper::seekWidgetByName(cell, "Image_Level"));
        imageLevel->loadTexture(image, TextureResType::PLIST);
        
        //open info
        auto imageLock = Helper::seekWidgetByName(cell, "Image_Lock");
        auto labelLevel = Helper::seekWidgetByName(cell, "Label_Level");
        auto open = levelData.open;
        if (open == 1)
        {
            imageLock->setVisible(false);
            labelLevel->setVisible(true);
            setLabelText(labelLevel, levelData.level_id);
        }
        else
        {
            imageLock->setVisible(true);
            labelLevel->setVisible(false);
        }
        char ch[32];
        for (int i = 1; i <= levelData.star; i++)
        {
            sprintf(ch, "Image_Star%d", i);
            auto star = Helper::seekWidgetByName(cell, ch);
            star->setVisible(true);
        }
        for (int j = levelData.star + 1; j <= 3; j++)
        {
            sprintf(ch, "Image_Star%d", j);
            auto star = Helper::seekWidgetByName(cell, ch);
            star->setVisible(false);
        }
        m_listView->pushBackCustomItem(cell);
    }
}

void LevelScene::backCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            AudioEnginMgr::playEffect(BACK);
            Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, WorldScene::createScene()));
        }
        break;
    default:
        break;
    }
}

void LevelScene::buyCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        break;
    default:
        break;
    }
}

void LevelScene::scrollViewCallBack(Ref* sender, ScrollviewEventType type)
{
    m_scrolling = true;
    this->stopAllActions();
    m_selectedItem->runAction(EaseBackInOut::create(ScaleTo::create(0.15f, 1.0f)));
}

void LevelScene::selectedItemEvent(Ref* sender, ListViewEventType type)
{
    ListView* view = dynamic_cast<ListView*>(sender);
    ssize_t index = view->getCurSelectedIndex();
    m_selectedItem = view->getItem(index);

    switch (type)
    {
    case LISTVIEW_ONSELECTEDITEM_START:
        {
            m_scrolling = false;
            this->stopAllActions();
            m_selectedItem->runAction(EaseBackInOut::create(ScaleTo::create(0.15f, 0.9f)));
        }
        break;
    case LISTVIEW_ONSELECTEDITEM_END:
        {
            if (!m_scrolling)
            {
                this->stopAllActions();
                m_selectedItem->runAction(EaseBackInOut::create(ScaleTo::create(0.15f, 1.0f)));
                DataMgr::levelIndex = index;
                if (DataMgr::getInstance()->getLevelData()[DataMgr::worldId][index].open == 1)
                {
                    AudioEnginMgr::playEffect(BUTTON);
                    //Director::getInstance()->replaceScene(TransitionFade::create(0.5f, LevelScene::createScene()));
                    //ÒªÇÐ»»µÄ¹Ø
                     int level = 1;
                     Global::getInstance()->setCurrentLevel(level);
                     Director::getInstance()->replaceScene(TransitionFade::create(0.5f,GameScene::createScene()));
                }
                else
                {
                    AudioEnginMgr::playEffect(LOCKED);
                }
            }
        }
        break;
    default:
        break;
    }
}
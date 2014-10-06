#include "MainScene.h"
#include "SelectMapScene.h"
#include "GameOverDialog.h"
#include "ExitDialog.h"
#include "DataMgr.h"
#include "HttpClientMgr.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>  
#include "platform/android/jni/JniHelper.h"  
#include <android/log.h>  
#include "MobClickCpp.h"
#endif

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}

MainScene::MainScene()
{
    
}

MainScene::~MainScene()
{
}

bool MainScene::init()
{
    if ( !Base::init() )
    {
        return false;
    }

    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/MainScene.ExportJson");
    this->addChild(m_view);

    initUI();

    m_listener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this); 
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener, this);

    if (DataMgr::bIsSpalshEnter)
    {
        DataMgr::bIsSpalshEnter = false;
    }

    return true;
}
void MainScene::initUI()
{
    auto panelRoot = Helper::seekWidgetByName(m_view, "Panel_Root");
    panelRoot->setSize(getVisableSize());
    panelRoot->setPosition(VisibleRect::leftBottom());

    //PLAY
    auto buttonPlay = Helper::seekWidgetByName(m_view, "Button_Play");
    buttonPlay->addTouchEventListener(this, toucheventselector(MainScene::playCallBack));

    addOwnAds();
}

void MainScene::playCallBack(Ref* sender, TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        AudioEnginMgr::getInstance()->playBtnEffect();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectMapScene::createScene()));
        break;
    default:
        break;
    }
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keycode,Event * pEvent) 
{ 
    if(EventKeyboard::KeyCode::KEY_RETURN == keycode || EventKeyboard::KeyCode::KEY_ESCAPE == keycode || EventKeyboard::KeyCode::KEY_BACKSPACE == keycode)
    {
        if (!DataMgr::modalShow)
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            umeng::MobClickCpp::event("user_exit");
            JniMethodInfo t;
            JniHelper::getStaticMethodInfo(t, "com/gameworld/up/hill/racing/Airpush", "airSmartWallAd", "()V");
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
#endif
            UtilHelper::showStartAppAd(2);
        }
    }
}
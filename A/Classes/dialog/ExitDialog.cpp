#include "ExitDialog.h"
#include "UtilHelper.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "MobClickCpp.h"
#endif

Scene* ExitDialog::createScene()
{
    auto scene = Scene::create();
    auto layer = ExitDialog::create();
    scene->addChild(layer);
    return scene;
}

ExitDialog::ExitDialog()
{

}

ExitDialog::~ExitDialog()
{
}

bool ExitDialog::init()
{
    if ( !Base::init() )
    {
        return false;
    }

    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/ExitDialog.ExportJson");
    this->addChild(m_view);

    initUI();
    
    return true;
}
void ExitDialog::initUI()
{
    //quit
    auto buttonQuit = Helper::seekWidgetByName(m_view, "Button_Quit");
    buttonQuit->addTouchEventListener(this, toucheventselector(ExitDialog::quitCallback));

    //quit
    auto buttonCancel = Helper::seekWidgetByName(m_view, "Button_Cancel");
    buttonCancel->addTouchEventListener(this, toucheventselector(ExitDialog::cancelCallback));
}

void ExitDialog::quitCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
        AudioEnginMgr::getInstance()->playBtnEffect();
        #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            umeng::MobClickCpp::end();
        #endif
            Director::getInstance()->end(); 
        }
        break;
    default:
        break;
    }
}

void ExitDialog::cancelCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            AudioEnginMgr::getInstance()->playBtnEffect();
            remove();
        }
        break;
    default:
        break;
    }
}
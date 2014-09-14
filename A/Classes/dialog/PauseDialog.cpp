#include "PauseDialog.h"
#include "UtilHelper.h"

Scene* PauseDialog::createScene()
{
    auto scene = Scene::create();
    auto layer = PauseDialog::create();
    scene->addChild(layer);
    return scene;
}

PauseDialog::PauseDialog()
{
}

PauseDialog::~PauseDialog()
{
}

bool PauseDialog::init()
{
    if ( !Base::init() )
    {
        return false;
    }

    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/PauseDialog.ExportJson");
    this->addChild(m_view);

    initUI();
    
    return true;
}
void PauseDialog::initUI()
{
    //resume
    auto resumeSettings = Helper::seekWidgetByName(m_view, "Button_Resume");
    resumeSettings->addTouchEventListener(this, toucheventselector(PauseDialog::resumeCallback));

    //retry
    auto retrySettings = Helper::seekWidgetByName(m_view, "Button_Retry");
    retrySettings->addTouchEventListener(this, toucheventselector(PauseDialog::retryCallback));

    //back
    auto backSettings = Helper::seekWidgetByName(m_view, "Button_Back");
    backSettings->addTouchEventListener(this, toucheventselector(PauseDialog::backCallback));
}

void PauseDialog::setResumeCallbackFunc(Ref* sender, SEL_CallFunc callbackFunc)
{
    m_callbackTarget =  sender;
    m_resumeCallbackFunc = callbackFunc;
}

void PauseDialog::setRestartCallbackFunc(Ref* sender, SEL_CallFunc callbackFunc)
{
    m_callbackTarget =  sender;
    m_restartCallbackFunc = callbackFunc;
}

void PauseDialog::setBackCallbackFunc(Ref* sender, SEL_CallFunc callbackFunc)
{
    m_callbackTarget =  sender;
    m_backCallbackFunc = callbackFunc;
}

void PauseDialog::resumeCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            if (m_callbackTarget && m_resumeCallbackFunc)
            {
                (m_callbackTarget->*m_resumeCallbackFunc)();
            }
            remove();
        }
        break;
    default:
        break;
    }
}

void PauseDialog::retryCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        if (m_callbackTarget && m_restartCallbackFunc)
            {
                (m_callbackTarget->*m_restartCallbackFunc)();
            }
            remove();
        break;
    default:
        break;
    }
}

void PauseDialog::backCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        if (m_callbackTarget && m_backCallbackFunc)
            {
                (m_callbackTarget->*m_backCallbackFunc)();
            }
            remove();
        break;
    default:
        break;
    }
}
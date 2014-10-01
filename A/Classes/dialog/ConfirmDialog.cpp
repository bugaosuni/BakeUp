#include "ConfirmDialog.h"
#include "UtilHelper.h"
#include "DailyTaskDialog.h"

Scene* ConfirmDialog::createScene()
{
    auto scene = Scene::create();
    auto layer = ConfirmDialog::create();
    scene->addChild(layer);
    return scene;
}

ConfirmDialog::ConfirmDialog()
{
    m_enough = false;
    m_info = "";
}

ConfirmDialog::~ConfirmDialog()
{
}

bool ConfirmDialog::init()
{
    if ( !Base::init() )
    {
        return false;
    }

    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/ConfirmDialog.ExportJson");
    this->addChild(m_view);

    initUI();
    
    return true;
}
void ConfirmDialog::initUI()
{
    //OK
    Button* okButton = dynamic_cast<Button*>(Helper::seekWidgetByName(m_view, "Button_OK"));
    okButton->addTouchEventListener(this, toucheventselector(ConfirmDialog::okCallback));
    if (!m_enough)
    {
        okButton->setTitleText("Get Coins");
    }
    

    //Cancel
    auto cancelButton = Helper::seekWidgetByName(m_view, "Button_Cancel");
    cancelButton->addTouchEventListener(this, toucheventselector(ConfirmDialog::cancelCallback));

    //info
    auto bitmapLabelInfo = Helper::seekWidgetByName(m_view, "Label_Info");
    setLabelText(bitmapLabelInfo, m_info.c_str());

}

void ConfirmDialog::setEnough(bool enough)
{
    m_enough =  enough;
}

void ConfirmDialog::setInfo(string info)
{
    m_info = info;
}

void ConfirmDialog::setCallbackFunc(Ref* sender, SEL_CallFunc callbackFunc)
{
    m_callbackTarget =  sender;
    m_callbackFunc = callbackFunc;
}

void ConfirmDialog::okCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            if (m_enough)
            {
                if (m_callbackTarget && m_callbackFunc)
                {
                    (m_callbackTarget->*m_callbackFunc)();
                    remove();
                }
            }
            else
            {
                DailyTaskDialog* dlg = new DailyTaskDialog();
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

void ConfirmDialog::cancelCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        remove();
        break;
    default:
        break;
    }
}
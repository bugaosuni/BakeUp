#include "ConfirmDialog.h"
#include "UtilHelper.h"

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
	auto okButton = Helper::seekWidgetByName(m_view, "Button_Ok");
	okButton->addTouchEventListener(this, toucheventselector(ConfirmDialog::okCallback));
	okButton->setVisible(m_enough);

	//Get
	auto getButton = Helper::seekWidgetByName(m_view, "Button_Get");
	getButton->addTouchEventListener(this, toucheventselector(ConfirmDialog::getCallback));
	getButton->setVisible(!m_enough);

	//Cancel
	auto cancelButton = Helper::seekWidgetByName(m_view, "Button_Cancel");
	cancelButton->addTouchEventListener(this, toucheventselector(ConfirmDialog::cancelCallback));

	//info
	auto bitmapLabelInfo = Helper::seekWidgetByName(m_view, "BitmapLabel_Info");
	setLabelText(bitmapLabelInfo, m_info.c_str());

	m_tips = Helper::seekWidgetByName(m_view, "BitmapLabel_Tips");
	m_tips->setScale(0);
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
			if (m_callbackTarget && m_callbackFunc)
			{
				(m_callbackTarget->*m_callbackFunc)();
			}
			remove();
		}
		break;
	default:
		break;
	}
}

void ConfirmDialog::getCallback(Ref* sender,TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			m_tips->stopAllActions();
			m_tips->runAction(Sequence::create(FadeIn::create(0.001f), EaseBackOut::create(ScaleTo::create(0.2f, 1.0f)), FadeOut::create(3.0f), 
				CallFunc::create(
				[&](){
				m_tips->setScale(0);
			}), NULL));
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
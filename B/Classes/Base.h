#ifndef __BASE_H__
#define __BASE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "VisibleRect.h"
#include "StringConst.h"
#include "AudioEnginMgr.h"
#include "HttpClientMgr.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio;
using namespace std;

class Base : public Layer
{
public:
	Base();
	virtual ~Base();
	CREATE_FUNC(Base);
	virtual bool init();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

	Size getVisableSize();
	void setLabelText(Widget* widget, string text);
	void setLabelText(Widget* widget, int text);
	void setLabelText(Node* node, string text);
	void setLabelText(Node* node, int text);
	void remove();
protected:
	void settingsCallback(Ref* sender,TouchEventType type);
	void addCallback(Ref* sender,TouchEventType type);
	void addOwnAds();
protected:
	Widget* m_view;
	EventListenerKeyboard* m_listener;
};

#endif // __BASE_H__

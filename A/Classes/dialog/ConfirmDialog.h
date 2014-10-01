#ifndef __CONFIRM_DIALOG_H__
#define __CONFIRM_DIALOG_H__
#include "Base.h"

class ConfirmDialog: public Base
{
public:
	ConfirmDialog();
	virtual ~ConfirmDialog();
	static Scene* createScene();
	virtual bool init();  
	CREATE_FUNC(ConfirmDialog);

	void initUI();
	void setEnough(bool enough);
	void setInfo(string info);
	void setCallbackFunc(Ref* sender, SEL_CallFunc callbackFunc);
private:
	void okCallback(Ref* sender,TouchEventType type);
	void getCallback(Ref* sender,TouchEventType type);
	void cancelCallback(Ref* sender,TouchEventType type);
private:
	bool m_enough;
	string m_info;
	Ref* m_callbackTarget;
	SEL_CallFunc m_callbackFunc;
};

#endif // __CONFIRM_DIALOG_H__

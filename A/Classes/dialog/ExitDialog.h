#ifndef __EXIT_DIALOG_H__
#define __EXIT_DIALOG_H__
#include "Base.h"

class ExitDialog: public Base
{
public:
	ExitDialog();
	virtual ~ExitDialog();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(ExitDialog);

	void initUI();
private:
	void quitCallback(Ref* sender,TouchEventType type);
	void cancelCallback(Ref* sender,TouchEventType type);
};

#endif // __EXIT_DIALOG_H__

#ifndef __PAUSE_DIALOG_H__
#define __PAUSE_DIALOG_H__
#include "Base.h"

class PauseDialog: public Base
{
public:
    PauseDialog();
    virtual ~PauseDialog();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(PauseDialog);

    void initUI();
    void setResumeCallbackFunc(Ref* sender, SEL_CallFunc callbackFunc);
    void setRestartCallbackFunc(Ref* sender, SEL_CallFunc callbackFunc);
    void setBackCallbackFunc(Ref* sender, SEL_CallFunc callbackFunc);
private:
    void resumeCallback(Ref* sender,TouchEventType type);
    void retryCallback(Ref* sender,TouchEventType type);
    void backCallback(Ref* sender,TouchEventType type);
private:
    Ref* m_callbackTarget;
    SEL_CallFunc m_resumeCallbackFunc;
    SEL_CallFunc m_restartCallbackFunc;
    SEL_CallFunc m_backCallbackFunc;
};

#endif // __PAUSE_DIALOG_H__

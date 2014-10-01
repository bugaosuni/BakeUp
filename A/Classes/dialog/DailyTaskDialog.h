#ifndef __DAILYTASK_DIALOG_H__
#define __DAILYTASK_DIALOG_H__
#include "Base.h"


class DailyTaskDialog: public Base
{
public:
    DailyTaskDialog();
    virtual ~DailyTaskDialog();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(DailyTaskDialog);

    void initUI();
private:
    void selectedItemEvent(Ref* pSender, ListView::EventType type);
    void getRewardCallback(Ref* sender,TouchEventType type);
    void backCallback(Ref* sender,TouchEventType type);

    int m_iListIndex;

    ListView* m_pDailyTaskListView;
};

#endif // __DAILYTASK_DIALOG_H__

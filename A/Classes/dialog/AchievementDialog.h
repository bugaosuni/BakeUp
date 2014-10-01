#ifndef __ACHIEVEMENT_DIALOG_H__
#define __ACHIEVEMENT_DIALOG_H__
#include "Base.h"

class AchievementDialog: public Base
{
public:
    AchievementDialog();
    virtual ~AchievementDialog();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(AchievementDialog);

    void initUI();
private:
    void selectedItemEvent(Ref* pSender, ListView::EventType type);
    void getRewardCallback(Ref* sender,TouchEventType type);
    void backCallback(Ref* sender,TouchEventType type);

    int m_iListIndex;

    ListView* m_pAchievementListView;
};

#endif // __ACHIEVEMENT_DIALOG_H__

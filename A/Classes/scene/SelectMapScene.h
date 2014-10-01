#ifndef __SELECTMAP_SCENE_H__
#define __SELECTMAP_SCENE_H__
#include "Base.h"

class SelectMapScene: public Base
{
public:
    SelectMapScene();
    virtual ~SelectMapScene();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(SelectMapScene);

    virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
    
    void initUI();
private:
    void goCallback(Ref* sender,TouchEventType type);
    void scrollViewCallBack(Ref* sender, ScrollviewEventType type);
    void selectedItemEvent(Ref* sender, ListViewEventType type);
    void initListView();
    void scrollListView(float offsetX);
    void selectEvent();
    void refreshUI();
private:
    Widget* m_labelGold;
    ListView* m_listView;
    bool m_scrolling;
    int m_selectedIndex;
};

#endif // __SELECTMAP_SCENE_H__

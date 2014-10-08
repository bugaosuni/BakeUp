#ifndef __WORLD_SCENE_H__
#define __WORLD_SCENE_H__
#include "Base.h"

class WorldScene: public Base
{
public:
    WorldScene();
    virtual ~WorldScene();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(WorldScene);

    virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
    
    void initUI();
private:
    void backCallback(Ref* sender,TouchEventType type);
    void buyCallback(Ref* sender,TouchEventType type);
    void scrollViewCallBack(Ref* sender, ScrollviewEventType type);
    void selectedItemEvent(Ref* sender, ListViewEventType type);
    void initListView();
    void scrollListView(float offsetX);
private:
    ListView* m_listView;
    bool m_scrolling;
    int m_selectedIndex;
};

#endif // __WORLD_SCENE_H__

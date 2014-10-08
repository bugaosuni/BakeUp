#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__
#include "Base.h"

class LevelScene: public Base
{
public:
    LevelScene();
    virtual ~LevelScene();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(LevelScene);

    virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
    
    void initUI();
private:
    void backCallback(Ref* sender,TouchEventType type);
    void buyCallback(Ref* sender,TouchEventType type);
    void scrollViewCallBack(Ref* sender, ScrollviewEventType type);
    void selectedItemEvent(Ref* sender, ListViewEventType type);
    void initListView();
private:
    ListView* m_listView;
    bool m_scrolling;
    Widget* m_selectedItem;
};

#endif // __LEVEL_SCENE_H__

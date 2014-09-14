#ifndef __SELECTCAR_SCENE_H__
#define __SELECTCAR_SCENE_H__
#include "Base.h"

class SelectCarScene: public Base
{
public:
	SelectCarScene();
	virtual ~SelectCarScene();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(SelectCarScene);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

	void initUI();
private:
	void backCallback(Ref* sender,TouchEventType type);
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

#endif // __SELECTCAR_SCENE_H__

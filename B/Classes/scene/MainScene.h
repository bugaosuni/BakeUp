#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__
#include "Base.h"

class MainScene: public Base
{
public:
    MainScene();
    virtual ~MainScene();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(MainScene);

    virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);
    
    void initUI();
private:
    void playCallBack(Ref* sender, TouchEventType type);
    void rateCallBack(Ref* sender, TouchEventType type);
    void musicCallBack(Ref* sender, TouchEventType type);
    void soundCallBack(Ref* sender, TouchEventType type);
private:
    Button* m_buttonMusic;
    Button* m_buttonSound;
    bool m_isPause;
};

#endif // __MAIN_SCENE_H__

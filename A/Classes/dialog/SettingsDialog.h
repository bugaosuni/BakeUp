#ifndef __SETTINGS_DIALOG_H__
#define __SETTINGS_DIALOG_H__
#include "Base.h"

class SettingsDialog: public Base
{
public:
	SettingsDialog();
	virtual ~SettingsDialog();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(SettingsDialog);

	void initUI();
private:
	void musicCallback(Ref* sender,TouchEventType type);
	void soundCallback(Ref* sender,TouchEventType type);
	void closeCallback(Ref* sender,TouchEventType type);
private:
	Widget* m_buttonMusic;
	Widget* m_buttonMusicOff;
	Widget* m_buttonSound;
	Widget* m_buttonSoundOff;
};

#endif // __SETTINGS_DIALOG_H__

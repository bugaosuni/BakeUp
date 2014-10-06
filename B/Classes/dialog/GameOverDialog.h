#ifndef __GAMEOVER_DIALOG_H__
#define __GAMEOVER_DIALOG_H__

#include "Base.h"

class GameOverDialog: public Base
{
public:
	GameOverDialog();
	virtual ~GameOverDialog();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(GameOverDialog);

	void initUI();
	void setDistance(int distance);
	void setCoins(int coins);
	void setOverType(string type);
	void setCallbackFunc(Ref* sender, SEL_CallFunc callbackFunc);
private:
	void showScreenshot(float dt);
	void cantouch();
	void playCallback(Ref* sender,TouchEventType type);
private:
	int m_distance;
	int m_coins;
	string m_type;
	Ref* m_callbackTarget;
	SEL_CallFunc m_callbackFunc;
	bool m_canTouch;
};

#endif // __GAMEOVER_DIALOG_H__

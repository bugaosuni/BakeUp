#ifndef __UPGRADECAR_SCENE_H__
#define __UPGRADECAR_SCENE_H__
#include "Base.h"

enum UPGRADETYPE
{
    e_type1 = 0,
    e_type2,
    e_type3,
    e_type4,
    e_type_count
};

struct PartData
{
    string	name;
    int		level;
    int		maxlevel;
};

class UpgradeCarScene: public Base
{
public:
    UpgradeCarScene();
    virtual ~UpgradeCarScene();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(UpgradeCarScene);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

    void initUI();
private:
    void upgradeCallback(Ref* sender,TouchEventType type);
    void backCallback(Ref* sender,TouchEventType type);
    void goCallback(Ref* sender,TouchEventType type);
    void clickCallback(Ref* sender,TouchEventType type);

    void initPartData();
    void initClicks();
	void refreshDesc();
	void refreshCost();
	void refreshLevel();
private:
	Widget* m_labelGold;
    Widget* m_bitmapLabelUpgrade;
    Label* m_labelDesc;
    Widget* m_bitmapLabelCost;
    UPGRADETYPE m_currType;
    vector<PartData> m_partData;
    vector<Button*> m_upgradeTypes;
	Widget* m_buttonUpgrade;
	int m_upgradeCost;
};

#endif // __UPGRADECAR_SCENE_H__

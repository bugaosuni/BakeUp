#include "SettingsDialog.h"
#include "UtilHelper.h"
#include "AudioEnginMgr.h"

Scene* SettingsDialog::createScene()
{
    auto scene = Scene::create();
    auto layer = SettingsDialog::create();
    scene->addChild(layer);
    return scene;
}

SettingsDialog::SettingsDialog()
{
    m_buttonMusic = nullptr;
    m_buttonMusicOff = nullptr;
    m_buttonSound = nullptr;
    m_buttonSoundOff = nullptr;
}

SettingsDialog::~SettingsDialog()
{
}

bool SettingsDialog::init()
{
    if ( !Base::init() )
    {
        return false;
    }

    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/SettingsDialog.ExportJson");
    this->addChild(m_view);

    initUI();
    
    return true;
}
void SettingsDialog::initUI()
{
    //music
    m_buttonMusic = Helper::seekWidgetByName(m_view, "Button_Music");
    m_buttonMusic->addTouchEventListener(this, toucheventselector(SettingsDialog::musicCallback));
    m_buttonMusic->setVisible(!UtilHelper::getFromBool(MUSIC_OFF));

    //musicoff
    m_buttonMusicOff = Helper::seekWidgetByName(m_view, "Button_Music_Off");
    m_buttonMusicOff->addTouchEventListener(this, toucheventselector(SettingsDialog::musicCallback));
    m_buttonMusicOff->setVisible(UtilHelper::getFromBool(MUSIC_OFF));

    //sound
    m_buttonSound = Helper::seekWidgetByName(m_view, "Button_Sound");
    m_buttonSound->addTouchEventListener(this, toucheventselector(SettingsDialog::soundCallback));
    m_buttonSound->setVisible(!UtilHelper::getFromBool(SOUND_OFF));

    //soundoff
    m_buttonSoundOff = Helper::seekWidgetByName(m_view, "Button_Sound_Off");
    m_buttonSoundOff->addTouchEventListener(this, toucheventselector(SettingsDialog::soundCallback));
    m_buttonSoundOff->setVisible(UtilHelper::getFromBool(SOUND_OFF));

    auto buttonClose = Helper::seekWidgetByName(m_view, "Button_Close");
    buttonClose->addTouchEventListener(this, toucheventselector(SettingsDialog::closeCallback));
}

void SettingsDialog::musicCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            AudioEnginMgr::getInstance()->playBtnEffect();
            bool music = UtilHelper::getFromBool(MUSIC_OFF);
           
            m_buttonMusic->setVisible(music);
            m_buttonMusicOff->setVisible(!music);
            UtilHelper::writeToBool(MUSIC_OFF, !music);
            
            if (music)
            {
                AudioEnginMgr::getInstance()->playBackgroundMusic();
            }
            else
            {
                AudioEnginMgr::getInstance()->stopBackgroundMusic();
            }

        
        }
        break;
    default:
        break;
    }
}

void SettingsDialog::soundCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            AudioEnginMgr::getInstance()->playBtnEffect();
            bool sound = UtilHelper::getFromBool(SOUND_OFF);
            m_buttonSound->setVisible(sound);
            m_buttonSoundOff->setVisible(!sound);
            UtilHelper::writeToBool(SOUND_OFF, !sound);
        }
        break;
    default:
        break;
    }
}

void SettingsDialog::closeCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            AudioEnginMgr::getInstance()->playBtnEffect();
            remove();
        }
        break;
    default:
        break;
    }
}
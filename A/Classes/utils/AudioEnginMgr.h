#ifndef __AUDIOENGINMGR_H__
#define __AUDIOENGINMGR_H__

#define GAS_EFFECT_MUSIC "music/gas.ogg"
#define BRAKE_EFFECT_MUSIC "music/brake.ogg"
#define PICK_COIN_EFFECT_MUSIC "music/pick_coin.ogg"
#define PICK_FUEL_EFFECT_MUSIC "music/pick_fuel.ogg"
#define BTN_EFFECT_MUSIC "music/btn.ogg"
#define SCREEN_SHOT_EFFECT_MUSIC "music/screenshot.ogg"
#define CRACK_EFFECT_MUSIC "music/crack.ogg"

#define BACKGROUND_MUSIC "music/background.ogg"
#define GAME_BACKGROUND_MUSIC "music/background.ogg"


class AudioEnginMgr
{
private:
    AudioEnginMgr();
    virtual ~AudioEnginMgr();
    static AudioEnginMgr* pInstance;

    class Garbo
    {
    public:
        ~Garbo()
        {
            if (AudioEnginMgr::pInstance)
            {
                delete AudioEnginMgr::pInstance;
            }
        }
    };
    static Garbo* garbo;

    int m_nGasEffect;
    int m_nBrakeEffect;

public:
    static AudioEnginMgr* getInstance();
    void initMusic();

    void playBackgroundMusic();
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();

    void playGasEffect(bool bIsLoop);
    void stopGasEffect();

    void playBrakeEffect(bool bIsLoop);
    void stopBrakeEffect();

    void playPickCoinEffect();
    void playPickFuelEffect();
    
    void playCrackEffect();
    void playScreenShotEffect();

    void playBtnEffect();

};
#endif /* __AUDIOENGINMGR_H__ */
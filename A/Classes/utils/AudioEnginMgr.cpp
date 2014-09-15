#include "AudioEnginMgr.h"
#include "SimpleAudioEngine.h"
#include "UtilHelper.h"
#include "StringConst.h"
#include "DataMgr.h"

using namespace CocosDenshion;

AudioEnginMgr* AudioEnginMgr::pInstance = new AudioEnginMgr();

AudioEnginMgr::AudioEnginMgr()
{
    m_nGasEffect = 0;
    m_nBrakeEffect = 0;
}

AudioEnginMgr::~AudioEnginMgr()
{
    if (pInstance)
        delete pInstance;
}

AudioEnginMgr* AudioEnginMgr::getInstance()
{
    return pInstance;
}

void AudioEnginMgr::initMusic()
{
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACKGROUND_MUSIC);
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GAME_BACKGROUND_MUSIC);

    SimpleAudioEngine::getInstance()->preloadEffect(GAS_EFFECT_MUSIC);
    SimpleAudioEngine::getInstance()->preloadEffect(BRAKE_EFFECT_MUSIC);
    SimpleAudioEngine::getInstance()->preloadEffect(PICK_COIN_EFFECT_MUSIC);

    SimpleAudioEngine::getInstance()->preloadEffect(BTN_EFFECT_MUSIC);
    SimpleAudioEngine::getInstance()->preloadEffect(PICK_FUEL_EFFECT_MUSIC);
    SimpleAudioEngine::getInstance()->preloadEffect(CRACK_EFFECT_MUSIC);
    SimpleAudioEngine::getInstance()->preloadEffect(SCREEN_SHOT_EFFECT_MUSIC);

    SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.3);
}

void AudioEnginMgr::playBackgroundMusic()
{
    if (DataMgr::bIsGameScene && !UtilHelper::getFromBool(MUSIC_OFF) && !DataMgr::bIsPause)
    {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSIC, true);
    }
    else if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() && !UtilHelper::getFromBool(MUSIC_OFF) && !DataMgr::bIsPause)
    {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(BACKGROUND_MUSIC, true);
    }
}
void AudioEnginMgr::stopBackgroundMusic()
{
    if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
    {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
}

void AudioEnginMgr::pauseBackgroundMusic()
{
    if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
    {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}
void AudioEnginMgr::resumeBackgroundMusic()
{
    if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
    {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
}


void AudioEnginMgr::playGasEffect(bool bIsLoop)
{
    if (!UtilHelper::getFromBool(SOUND_OFF))
    {
        m_nGasEffect = SimpleAudioEngine::getInstance()->playEffect(GAS_EFFECT_MUSIC, bIsLoop);
    }
}

void AudioEnginMgr::stopGasEffect()
{
if (!UtilHelper::getFromBool(SOUND_OFF))
    {
        SimpleAudioEngine::getInstance()->stopEffect(m_nGasEffect);
    }
}

void AudioEnginMgr::playBrakeEffect(bool bIsLoop)
{
    if (!UtilHelper::getFromBool(SOUND_OFF))
    {
        m_nBrakeEffect = SimpleAudioEngine::getInstance()->playEffect(BRAKE_EFFECT_MUSIC, bIsLoop);
    }
}

void AudioEnginMgr::stopBrakeEffect()
{
    if (!UtilHelper::getFromBool(SOUND_OFF))
    {
        SimpleAudioEngine::getInstance()->stopEffect(m_nBrakeEffect);
    }
}

void AudioEnginMgr::playPickCoinEffect()
{
    if (!UtilHelper::getFromBool(SOUND_OFF))
    {
        SimpleAudioEngine::getInstance()->playEffect(PICK_COIN_EFFECT_MUSIC);
    }
}

void AudioEnginMgr::playPickFuelEffect()
{
    if (!UtilHelper::getFromBool(SOUND_OFF))
    {
        SimpleAudioEngine::getInstance()->playEffect(PICK_FUEL_EFFECT_MUSIC);
    }
}

void AudioEnginMgr::playCrackEffect()
{
    if (!UtilHelper::getFromBool(SOUND_OFF))
    {
        SimpleAudioEngine::getInstance()->playEffect(CRACK_EFFECT_MUSIC);
    }
}

void AudioEnginMgr::playScreenShotEffect()
{
    if (!UtilHelper::getFromBool(SOUND_OFF))
    {
        SimpleAudioEngine::getInstance()->playEffect(SCREEN_SHOT_EFFECT_MUSIC);
    }
}


void AudioEnginMgr::playBtnEffect()
{
     if (!UtilHelper::getFromBool(SOUND_OFF))
    {
        SimpleAudioEngine::getInstance()->playEffect(BTN_EFFECT_MUSIC);
    }
}
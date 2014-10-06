LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Base.cpp \
				   ../../Classes/scene/SplashScene.cpp \
				   ../../Classes/scene/MainScene.cpp \
				   ../../Classes/scene/SelectMapScene.cpp \
				   ../../Classes/scene/SelectCarScene.cpp \
				   ../../Classes/scene/UpgradeCarScene.cpp \
				   ../../Classes/scene/GameScene.cpp \
				   ../../Classes/dialog/PauseDialog.cpp \
				   ../../Classes/dialog/SettingsDialog.cpp \
				   ../../Classes/dialog/ConfirmDialog.cpp \
				   ../../Classes/dialog/GameOverDialog.cpp \
                   ../../Classes/dialog/ExitDialog.cpp \
				   ../../Classes/utils/UtilHelper.cpp \
				   ../../Classes/utils/HttpClientMgr.cpp \
				   ../../Classes/utils/VisibleRect.cpp \
				   ../../Classes/utils/ContactListener.cpp \
				   ../../Classes/utils/DataMgr.cpp \
				   ../../Classes/utils/GB2ShapeCache-x.cpp \
				   ../../Classes/utils/AudioEnginMgr.cpp \
                   ../../Classes/utils/GLES-Render.cpp \
				   ../../Classes/view/TerrainView.cpp \
                   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/scene
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/dialog
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/view
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/utils

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += mobclickcpp_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,libMobClickCpp)
$(call import-module,curl/prebuilt/android)
$(call import-module,audio/android)

$(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
$(call import-module,network)
$(call import-module,extensions)

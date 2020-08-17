LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_LIB_TYPE        := STATIC
OPENCV_INSTALL_MODULES := on
OPENCV_CAMERA_MODULES  := on

OPENCV_MK_PATH := ..\OpenCV-android-sdk\sdk\native\jni

include $(OPENCV_MK_PATH)/OpenCV.mk

LOCAL_MODULE := appnativecpp_opencv_jni
LOCAL_SRC_FILES := appnative.cpp Main.cpp Globals.cpp OSNet.c OSCriticalSection.c OSThread.c OSMisc.c OSTime.c OSCore.c
LOCAL_LDLIBS += -llog -ldl

# -x c++ because of OpenCV...
LOCAL_CFLAGS += -Wall -Wextra -D DISABLE_GUI_REMOTEWEBCAMMULTISRV -D DISABLE_TIMER_RECORDING -D DISABLE_CUSTOM_BAUDRATE -D USE_OPENCV_HIGHGUI_CPP_API -x c++

include $(BUILD_SHARED_LIBRARY)

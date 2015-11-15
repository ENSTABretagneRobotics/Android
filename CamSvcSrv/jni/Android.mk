LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_LIB_TYPE        := STATIC
OPENCV_INSTALL_MODULES := on
OPENCV_CAMERA_MODULES  := on

OPENCV_MK_PATH := ..\OpenCV-2.4.9-android-sdk\sdk\native\jni

include $(OPENCV_MK_PATH)/OpenCV.mk

LOCAL_MODULE := appnativecpp_opencv_jni
LOCAL_SRC_FILES := appnative.cpp Main.c Globals.c OSNet.c OSCriticalSection.c OSThread.c OSMisc.c OSTime.c OSCore.c
LOCAL_LDLIBS += -llog -ldl

# -x c++ because of OpenCV...
LOCAL_CFLAGS += -Wall -Wextra -DDISABLE_GUI_REMOTEWEBCAMMULTISRV -DDISABLE_TIMER_RECORDING -DOPENCV249 -x c++

include $(BUILD_SHARED_LIBRARY)

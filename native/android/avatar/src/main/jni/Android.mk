LOCAL_PATH := $(call my-dir)
DEPOT_ROOT := ../../../../../..
MY_APP_PATH := $(LOCAL_PATH)

JSON_LIB_PATH := $(LOCAL_PATH)
MB_LIB_PATH := $(LOCAL_PATH)

include $(CLEAR_VARS)
include $(JSON_LIB_PATH)/JSONLib.mk
include $(MB_LIB_PATH)/MsgBrokerLib.mk

include $(CLEAR_VARS)
LOCAL_PATH := $(MY_APP_PATH)

LOCAL_MODULE := nativepart
LOCAL_SRC_FILES := com_ford_avarsdl_jsonserver_MBWrapper.cpp

#path for libjson headers
LOCAL_C_INCLUDES += $(JSON_INCLUDE)
#path for message broker headers
LOCAL_C_INCLUDES += $(MB_INCLUDE_1)
LOCAL_C_INCLUDES += $(MB_INCLUDE_2)

LOCAL_SHARED_LIBRARIES := jsoncpp MsgBroker
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)


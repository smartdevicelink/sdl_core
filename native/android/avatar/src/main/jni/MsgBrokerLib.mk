LOCAL_PATH := $(call my-dir)

MB_SOURCES := $(DEPOT_ROOT)/common/MessageBroker/src/lib_messagebroker
MB_INCLUDE_1 := $(LOCAL_PATH)/../../../../../../common/MessageBroker/include
MB_INCLUDE_2 := $(LOCAL_PATH)/../../../../../../common/MessageBroker/src/lib_messagebroker

#build MsgBroker
include $(CLEAR_VARS)

LOCAL_MODULE := MsgBroker
LOCAL_SRC_FILES := $(MB_SOURCES)/CMessageBroker.cpp
LOCAL_SRC_FILES += $(MB_SOURCES)/CMessageBrokerRegistry.cpp
LOCAL_SRC_FILES += $(MB_SOURCES)/system.cpp

#path for libjson headers
LOCAL_C_INCLUDES := $(JSON_INCLUDE)
#path for message broker headers
LOCAL_C_INCLUDES += $(MB_INCLUDE_1)
LOCAL_C_INCLUDES += $(MB_INCLUDE_2)
 
LOCAL_SHARED_LIBRARIES := jsoncpp
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

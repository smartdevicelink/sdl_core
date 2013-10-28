LOCAL_PATH := $(call my-dir)

JSON_INCLUDE := $(LOCAL_PATH)/../../../../../../common/jsoncpp/include
JSON_SOURCES := $(DEPOT_ROOT)/common/jsoncpp/src/lib_json


include $(CLEAR_VARS)

LOCAL_MODULE := jsoncpp

LOCAL_SRC_FILES := $(JSON_SOURCES)/json_writer.cpp
LOCAL_SRC_FILES += $(JSON_SOURCES)/json_reader.cpp
LOCAL_SRC_FILES += $(JSON_SOURCES)/json_value.cpp

LOCAL_C_INCLUDES += $(JSON_INCLUDE)
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

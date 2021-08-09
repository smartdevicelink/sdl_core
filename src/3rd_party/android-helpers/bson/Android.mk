LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(info "Building emhashmap")

LOCAL_MODULE := emhashmap
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/src/emhashmap/*.c)
LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/src/emhashmap
LOCAL_CPP_FEATURES := rtti exceptions

include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)

$(info "Building bson")

LOCAL_MODULE := bson
LOCAL_SHARED_LIBRARIES := emhashmap
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/src/*.c)
LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/src
LOCAL_CPP_FEATURES := rtti exceptions

include $(BUILD_SHARED_LIBRARY)
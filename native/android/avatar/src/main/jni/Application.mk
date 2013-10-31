APP_STL := gnustl_static
#we want to use exceptions
APP_CPPFLAGS += -fexceptions
#we want RTTI
APP_CPPFLAGS += -frtti

APP_MODULES := jsoncpp MsgBroker nativepart
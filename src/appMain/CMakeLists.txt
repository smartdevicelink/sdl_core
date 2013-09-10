set(LIBRARIES
  ApplicationManager
  HMI_API
  MOBILE_API
  SmartObjects
  formatters
  ProtocolHandler
  connectionHandler
  TransportManager
  HMIMessageHandler
  MessageBroker
  MessageBrokerClient
  MessageBrokerServer
  Utils
  encryption
  jsoncpp
  bluetooth
  pthread
  rt
  avahi-client
  avahi-common
  ConfigProfile
  MobileMessageHandler
  RequestWatchdog
  AudioManager
#  usb-1.0
)

# required to find 'glibconfig.h'
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})

include_directories (
  ./
  ../components/protocol_handler/include/
  ../thirdPartyLibs/jsoncpp/include/
  ../components/mobile_message_handler/include
  ../components/application_manager/include
  ../components/formatters/include
  ../components/AppMgr/include
  ../components/transport_manager/include
  ../components/config_profile/include
  ../components/utils/include/
  ../components/connection_handler/include/
  ../components/hmi_message_handler/include
  ../components/request_watchdog/include
  ../components/smart_objects/include/
  ../components/audio_manager/include/
  ../utils/include/
  ../thirdPartyLibs/MessageBroker/include
  ../thirdPartyLibs/encryption/include
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_BINARY_DIR}/src/components/dbus_schema/include/
  ${CMAKE_BINARY_DIR}
  /usr/lib/i386-linux-gnu/glib-2.0/include
  /usr/lib/x86_64-linux-gnu/glib-2.0/include/
  ${GSTREAMER_gst_INCLUDE_DIR}
  ${GLIB_glib_2_INCLUDE_DIR}
)

set (SOURCES
  appMain.cpp
)

if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build. Options are: None, Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()

add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/audio.8bit.wav DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/smartDeviceLink.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/policy_table.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/hmi_link "${CMAKE_HOME_DIRECTORY}/src/components/HMI/index.html")

install (TARGETS ${PROJECT} DESTINATION bin)


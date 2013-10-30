IF (${WEB_HMI})
set (BROKER_LIBRARIES
  MessageBrokerClient
  MessageBrokerServer
)
ENDIF (${WEB_HMI})

cmake_policy(PUSH)
# make link_directories() treat paths relative to the source dir
# info: cmake --help-policy CMP0015
cmake_policy(SET CMP0015 NEW)
link_directories(../thirdPartyLibs/libusbx)
cmake_policy(POP)

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
  MessageBroker
  ${BROKER_LIBRARIES}
  MediaManager
  Libusb-1.0.16
)

# required to find 'glibconfig.h'
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})

include_directories (
  ${GSTREAMER_gst_INCLUDE_DIR}
  ${GLIB_glib_2_INCLUDE_DIR}
  ./
  ../components/protocol_handler/include/
  ../thirdPartyLibs/jsoncpp/include/
  ../components/mobile_message_handler/include
  ../components/application_manager/include
  ../components/formatters/include
  ../components/transport_manager/include
  ../components/config_profile/include
  ../components/utils/include/
  ../components/connection_handler/include/
  ../components/hmi_message_handler/include
  ../components/request_watchdog/include
  ../components/smart_objects/include/
  ../components/media_manager/include/
  ../utils/include/
  ../thirdPartyLibs/MessageBroker/include
  ../thirdPartyLibs/encryption/include
  ${CMAKE_SOURCE_DIR}/src/components/
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_SOURCE_DIR}/src/components/dbus/include/
  ${CMAKE_SOURCE_DIR}
)

set (SOURCES
  appMain.cpp
  life_cycle.cc
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

IF (${QT_HMI})
  execute_process(
    COMMAND ${CMAKE_SOURCE_DIR}/FindQt5.sh binary qmlscene
    OUTPUT_VARIABLE qmlscene_binary
  )
  set(main_qml "${CMAKE_HOME_DIRECTORY}/src/components/qt_hmi/qml_model/MainWindow.qml")
  set(plugins_dir "${CMAKE_HOME_DIRECTORY}/src/components/qt_hmi/qml_model")
  file(WRITE ${CMAKE_HOME_DIRECTORY}/start_hmi.sh
             "#!/bin/sh\n${qmlscene_binary} ${main_qml} -I ${plugins_dir}")
  file(COPY ${CMAKE_HOME_DIRECTORY}/start_hmi.sh
       DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
       FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
            GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
       NO_SOURCE_PERMISSIONS)
  file(REMOVE ${CMAKE_HOME_DIRECTORY}/start_hmi.sh)
ENDIF (${QT_HMI})

IF (${WEB_HMI})
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/hmi_link "${CMAKE_HOME_DIRECTORY}/src/components/HMI/index.html")
ENDIF (${WEB_HMI})



install (TARGETS ${PROJECT} DESTINATION bin)


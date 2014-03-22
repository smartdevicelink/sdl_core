IF (HMIADAPTER STREQUAL "messagebroker")
set (BROKER_LIBRARIES
  MessageBrokerClient
  MessageBrokerServer
)
ENDIF ()

cmake_policy(PUSH)
# make link_directories() treat paths relative to the source dir
# info: cmake --help-policy CMP0015
cmake_policy(SET CMP0015 NEW)
link_directories(../thirdPartyLibs/libusbx)
cmake_policy(POP)

if (EXTENDED_MEDIA_MODE)
set(default_media_inc
${GSTREAMER_gst_INCLUDE_DIR}
)
else(EXTENDED_MEDIA_MODE)
set(default_media_inc
)
endif()

set(LIBRARIES
  ApplicationManager
  HMI_API
  MOBILE_API
  v4_protocol_v1_2_no_extra
  SmartObjects
  formatters
  ProtocolHandler
  connectionHandler
  TransportManager
  HMIMessageHandler
  MessageBroker
  ${BROKER_LIBRARIES}
  Utils
  encryption
  jsoncpp
  ConfigProfile
  RequestWatchdog
  MediaManager
  Resumption	
  policies
  apr-1
  aprutil-1
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND LIBRARIES pthread)
endif()

if (BUILD_BT_SUPPORT)
	list(APPEND LIBRARIES bluetooth)
endif()
if (BUILD_AVAHI_SUPPORT)
	list(APPEND LIBRARIES avahi-client avahi-common)
endif()
if (BUILD_USB_SUPPORT)
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND LIBRARIES Libusb-1.0.16)
endif()
endif()

include_directories (
  ./
  ../components/protocol_handler/include/
  ../thirdPartyLibs/jsoncpp/include/
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
  ../components/policies/include/
  ../thirdPartyLibs/MessageBroker/include
  ../thirdPartyLibs/encryption/include
  ${CMAKE_SOURCE_DIR}/src/components/
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_SOURCE_DIR}/src/components/dbus/include/
  ${CMAKE_SOURCE_DIR}
  ${default_media_inc}
)

set (SOURCES
  main.cc
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
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/hmi_capabilities.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wp1_policy_table.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (BUILD_TESTS)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wp1_policy_table.json DESTINATION ${CMAKE_BINARY_DIR}/test/)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties DESTINATION ${CMAKE_BINARY_DIR}/test/)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/smartDeviceLink.ini DESTINATION ${CMAKE_BINARY_DIR}/test/)
endif()

if (${QT_HMI})
  set(main_qml "hmi/MainWindow.qml")
  set(plugins_dir "hmi/plugins")

  if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
    set(start_command "./start-qml.sh")
    set(command_arguments "${main_qml} ${plugins_dir}")
  else ()
    execute_process(
      COMMAND ${CMAKE_SOURCE_DIR}/FindQt.sh -v ${qt_version} -b qmlscene
      OUTPUT_VARIABLE start_command
    )
    set(command_arguments "${main_qml} -I ${plugins_dir}")
  endif ()

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/start_hmi.sh
    "#!/bin/sh\n${start_command} ${command_arguments}\n")
elseif (${WEB_HMI})
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/hmi_link
    "${CMAKE_HOME_DIRECTORY}/src/components/HMI/index.html")
endif ()

# Install rules
install(TARGETS ${PROJECT} DESTINATION bin)
install(
  FILES log4cxx.properties audio.8bit.wav test.txt smartDeviceLink.ini policy_table.json
    hmi_capabilities.json wp1_policy_table.json
  DESTINATION bin
)
if (${WEB_HMI})
  if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hmi_link DESTINATION bin)
  endif ()
elseif (${QT_HMI})
  if (CMAKE_SYSTEM_NAME STREQUAL "QNX")
    install(FILES start-qml.sh DESTINATION bin
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
        GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
  endif ()
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/start_hmi.sh DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
      GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif ()

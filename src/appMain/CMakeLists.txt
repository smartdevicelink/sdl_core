if(BUILD_ONLY_TM)
add_definitions(-DBUILD_ONLY_TM)
set(LIBRARIES
  ApplicationManager
  HMI_API
  SmartObjects
  #JSONHandler
  #ProtocolHandler
  #connectionHandler
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
  ConfigProfile
  MobileMessageHandler
  RequestWatchdog
)
include_directories (
  ./
  ../components/protocol_handler/include/
  ../thirdPartyLibs/jsoncpp/include/
  ../components/mobile_message_handler/include
  ../components/application_manager/include
  ../components/JSONHandler/include
  ../components/AppMgr/include
  ../components/transport_manager/include
  ../components/config_profile/include
  ../components/utils/include/
  ../components/connection_handler/include/
  ../components/hmi_message_handler/include
  ../components/request_watchdog/include
  ../components/SmartObjects/include/
  ../utils/include/
  ../thirdPartyLibs/MessageBroker/include
  ../thirdPartyLibs/encryption/include
  ${CMAKE_BINARY_DIR}/src/components/
)
else()
set(LIBRARIES
  ApplicationManager
  HMI_API
  SmartObjects
  JSONHandler
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
  ConfigProfile
  MobileMessageHandler
  RequestWatchdog
)
include_directories (
  ./
  ../components/protocol_handler/include/
  ../thirdPartyLibs/jsoncpp/include/
  ../components/mobile_message_handler/include
  ../components/application_manager/include
  ../components/JSONHandler/include
  ../components/AppMgr/include
  ../components/TransportManager/include
  ../components/config_profile/include
  ../components/utils/include/
  ../components/connection_handler/include/
  ../components/hmi_message_handler/include
  ../components/request_watchdog/include
  ../components/SmartObjects/include/
  ../utils/include/
  ../thirdPartyLibs/MessageBroker/include
  ../thirdPartyLibs/encryption/include
  ${CMAKE_BINARY_DIR}/src/components/
)
endif()


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

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/hmi_link "${CMAKE_HOME_DIRECTORY}/src/components/HMI/index.html")

install (TARGETS ${PROJECT} DESTINATION bin)


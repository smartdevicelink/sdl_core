set(LIBRARIES
  AppMgr
  JSONHandler
  ProtocolHandler
  connectionHandler
  TransportManager
  MessageBroker
  MessageBrokerClient
  MessageBrokerServer
  Utils
  encryption
  jsoncpp
  bluetooth
  pthread
  rt
)

include_directories (
  ./
  ../components/protocol_handler/include/
  ../thirdPartyLibs/jsoncpp/include/
  ../components/JSONHandler/include
  ../components/AppMgr/include
  ../components/TransportManager/include
  ../components/utils/include/
  ../components/ConnectionHandler/include/
  ../thirdPartyLibs/MessageBroker/include
  ../thirdPartyLibs/encryption/include
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

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/hmi_link "${CMAKE_HOME_DIRECTORY}/src/components/HMI/index.html")

install (TARGETS ${PROJECT} DESTINATION bin)


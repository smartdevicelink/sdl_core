set(LIBRARIES
  AppMgr
  JSONHandler
  #TransportManager
  ProtocolHandler
  MessageBroker
  MessageBrokerClient
  MessageBrokerServer
  jsoncpp
  bluetooth
  pthread
  log4cplus
  rt
)

include_directories (
  ./
  ../thirdPartyLibs/axisCore/utils/misc
  ../thirdPartyLibs/axisCore/include
  ../thirdPartyLibs/axisCore/src
  ../thirdPartyLibs/axisCore/
  ../thirdPartyLibs/axisCore/transport/bt
  ../thirdPartyLibs/jsoncpp/include/
  ../components/JSONHandler/include
  ../components/AppMgr/include
  ../components/TransportManager/include
  ../thirdPartyLibs/MessageBroker/include
  ../thirdPartyLibs/logger/include
  ../thirdPartyLibs/logger/log4cplus-1.1.0/include
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
  appMain.cpp
  CBTAdapter.cpp
  CAppTester.cpp
)

if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build. Options are: None, Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()

add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cplus.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
execute_process(COMMAND ln -s -f ${CMAKE_HOME_DIRECTORY}/src/components/HMI/index.html
                                 ${CMAKE_CURRENT_BINARY_DIR}/hmi_link)

install (TARGETS ${PROJECT} DESTINATION bin)


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
)

set (SOURCES
  appMain.cpp
  CBTAdapter.cpp
  CAppTester.cpp
)

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
  SET_SOURCE_FILES_PROPERTIES(
    appMain.cpp
    CBTAdapter.cpp
    CAppTester.cpp
    PROPERTIES
    CMAKE_CXX_FLAGS "-O0 -g3 -ggdb3"
    CMAKE_EXE_LINKER_FLAGS "-m"
  )
endif()

add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cplus.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install (TARGETS ${PROJECT} DESTINATION bin)


set (PROJECT testAppManager)
project (${PROJECT})

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

include_directories (../../../src/components/AppMgr/include/
					 ../../../src/components/JSONHandler/include/
					 ../../../src/thirdPartyLibs/MessageBroker/include/
					 ../../../src/thirdPartyLibs/jsoncpp/include/
					)

set (SOURCES
		main.cpp
	)

if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build. Options are: None, Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()

add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES})

install (TARGETS ${PROJECT} DESTINATION bin)

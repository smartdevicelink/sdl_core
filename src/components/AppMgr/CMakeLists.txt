include_directories (
  ./include/
  ../JSONHandler/include/
  ../../thirdPartyLibs/MessageBroker/include/
  ../../thirdPartyLibs/jsoncpp/include/
  ../../thirdPartyLibs/axisCore/
  ../../thirdPartyLibs/axisCore/include/
  ../../thirdPartyLibs/axisCore/src/
  ../../thirdPartyLibs/logger/include/
  ../../thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
    ./src/SubscribeButton.cpp
    ./src/IAppCommand.cpp
    ./src/AppMgr.cpp
    ./src/AppFactory.cpp
    ./src/AppMgrRegistry.cpp
    ./src/IApplication.cpp
    ./src/Application.cpp
    ./src/AppMgrCore.cpp
    ./src/AppPolicy.cpp
    ./src/RegistryItem.cpp
)

if(CMAKE_COMPILER_IS_GNUCXX)
  SET_SOURCE_FILES_PROPERTIES(
    ./src/SubscribeButton.cpp
    ./src/IAppCommand.cpp
    ./src/AppMgr.cpp
    ./src/AppFactory.cpp
    ./src/AppMgrRegistry.cpp
    ./src/IApplication.cpp
    ./src/Application.cpp
    ./src/AppMgrCore.cpp
    ./src/AppPolicy.cpp
    ./src/RegistryItem.cpp
    PROPERTIES
    CMAKE_CXX_FLAGS "-O0 -g3 -ggdb3 -std=c++0x"
    CMAKE_EXE_LINKER_FLAGS "-m"
  )
endif()

add_library("AppMgr" ${SOURCES})
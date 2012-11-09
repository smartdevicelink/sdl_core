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
    ./src/AppMgrCoreQueues.cpp
    ./src/AppPolicy.cpp
    ./src/RegistryItem.cpp
    ./src/ButtonMapping.cpp
    ./src/CommandMapping.cpp
    ./src/MessageMapping.cpp
    ./src/ButtonCapabilities.cpp
    ./src/HMIHandler.cpp
    ./src/MobileHandler.cpp
)

add_library("AppMgr" ${SOURCES})

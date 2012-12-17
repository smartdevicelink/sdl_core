include_directories (
    ./include/
    ../JSONHandler/include/
    ../ConnectionHandler/include/
    ../ProtocolHandler/include/
    ../TransportManager/include/
    ../Utils/include/
    ../../thirdPartyLibs/MessageBroker/include/
    ../../thirdPartyLibs/jsoncpp/include/
    ../../thirdPartyLibs/logger/include/
    ../../thirdPartyLibs/logger/log4cplus-1.1.0/include/
    ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
    ./src/SubscribeButton.cpp
    ./src/IAppCommand.cpp
    ./src/AppMgr.cpp
    ./src/AppMgrRegistry.cpp
    ./src/Application.cpp
    ./src/Application_v1.cpp
    ./src/Application_v2.cpp
    ./src/AppMgrCore.cpp
    ./src/AppMgrCoreQueues.cpp
    ./src/AppPolicy.cpp
    ./src/RegistryItem.cpp
    ./src/ButtonMapping.cpp
    ./src/CommandMapping.cpp
    ./src/MessageMapping.cpp
    ./src/MenuMapping.cpp
    ./src/CapabilitesContainer.cpp
    ./src/HMIHandler.cpp
    ./src/MobileHandler.cpp
    ./src/ConnectionHandler.cpp
    ./src/RequestMapping.cpp
    ./src/SyncPManager.cpp
    ./src/AppMenus.cpp
    ./src/AppChoiceSets.cpp
    ./src/DeviceList.cpp
    ./src/DeviceHandler.cpp
)

add_library("AppMgr" ${SOURCES})

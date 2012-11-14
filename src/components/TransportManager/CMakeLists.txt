include_directories (
  ./include
  ../../thirdPartyLibs/logger/include/
  ../../thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
    ./src/SDeviceInfo.cpp
    ./src/IDeviceAdapter.cpp
    ./src/IDeviceAdapterListener.cpp
    ./src/IHandleGenerator.cpp
    ./src/ITransportManager.cpp
    ./src/CBluetoothAdapter.cpp
    ./src/CDeviceAdapter.cpp
    ./src/CTransportManager.cpp
    ./src/SInternalDeviceInfo.cpp
    ./src/ITransportManagerDeviceListener.cpp
    ./src/ITransportManagerDataListener.cpp
)

add_library("TransportManager" ${SOURCES})
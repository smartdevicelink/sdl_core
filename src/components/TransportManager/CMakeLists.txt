include_directories (
  ./include
  ../utils/include/
)

set (SOURCES
    ./src/SDeviceInfo.cpp
    ./src/IDeviceAdapter.cpp
    ./src/IDeviceAdapterListener.cpp
    ./src/IHandleGenerator.cpp
    ./src/ITransportManager.cpp
    ./src/CDeviceAdapter.cpp
    ./src/CTransportManager.cpp
    ./src/CTCPAdapter.cpp
    ./src/SInternalDeviceInfo.cpp
    ./src/ITransportManagerDeviceListener.cpp
    ./src/ITransportManagerDataListener.cpp
)

add_library("TransportManager" ${SOURCES})

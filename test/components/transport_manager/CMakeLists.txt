include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/TransportManager/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/components/utils/include/
)

set (SOURCES
  ./HandleGeneratorsTest.cpp
  ./ListenersTest.cpp
  ./TcpSimulator.cpp
  ./TransportManagerTestWithCorrectDeviceAdapter.cpp
  ./TransportManagerTestWithHighDataTransferRate.cpp
  ./TransportManagerTestWithIncorrectDeviceAdapter.cpp
  ./TransportManagerTestWithMultipleClients.cpp
  ./TransportManagerTestWithMultipleDeviceAdapters.cpp
  ./TransportManagerTestWithMultiThreadedClient.cpp
)

add_library("test_TransportManagerTest" ${SOURCES})

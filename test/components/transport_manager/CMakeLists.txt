include_directories (
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/components/TransportManager/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/logger/include/
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
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

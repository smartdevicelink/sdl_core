include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/TransportManager/include
)

# Sources are separated for each Transport Manager test
set (SOURCES_LISTENERS_TEST
    ./ListenersTest.cpp
)

set (SOURCES_HANDLEGENERATORS_TEST
    ./HandleGeneratorsTest.cpp
)

# Sources for the TCP Simulator
set (SOURCES_TCP_SIMULATOR
    ./TcpSimulator.cpp
)

# Sources for the Draft TransportManager test
set (SOURCES_DRAFT_TEST
    ./TransportManagerDraftTest.cpp
)

# Sources for the TransportManager test with correct device adapter behavior
set (SOURCES_TEST_WITH_CORRECT_DEVICE_ADAPTER
    ./TransportManagerTestWithCorrectDeviceAdapter.cpp
)

# Sources for the TransportManager test with incorrect device adapter behavior
set (SOURCES_TEST_WITH_INCORRECT_DEVICE_ADAPTER ./TransportManagerTestWithIncorrectDeviceAdapter.cpp)

# Sources for the TransportManager test with multiple device adapters
set (SOURCES_TEST_WITH_MULTIPLE_DEVICE_ADAPTERS ./TransportManagerTestWithMultipleDeviceAdapters.cpp)

# Sources for the TransportManager test with high data transfer rate
set (SOURCES_TEST_WITH_HIGH_DATA_TRANSFER_RATE ./TransportManagerTestWithHighDataTransferRate.cpp)

# Source for the TransportManager test with multi threaded client
set (SOURCES_TEST_WITH_MULTI_THREADED_CLIENT ./TransportManagerTestWithMultiThreadedClient.cpp)

# Source for the TransportManager test with multiple clients
set (SOURCES_TEST_WITH_MULTIPLE_CLIENTS ./TransportManagerTestWithMultipleClients.cpp)

# Libraries seeems to be the same for all Transport Manager test
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    TransportManager
    bluetooth
)

add_executable("test_TransportManagerListeners" ${SOURCES_LISTENERS_TEST})
target_link_libraries("test_TransportManagerListeners" ${LIBRARIES})
add_test(test_TransportManagerListeners test_TransportManagerListeners)

add_executable("test_TransportManagerHandleGenerators" ${SOURCES_HANDLEGENERATORS_TEST})
target_link_libraries("test_TransportManagerHandleGenerators" ${LIBRARIES})
add_test(test_TransportManagerHandleGenerators test_TransportManagerHandleGenerators)

add_executable("test_TransportManagerTCPSimulator" ${SOURCES_TCP_SIMULATOR})
# It doesn't require any additional libraries at the moment

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cxx.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable("test_TransportManagerTestWithCorrectDeviceAdapter" ${SOURCES_TEST_WITH_CORRECT_DEVICE_ADAPTER})
target_link_libraries("test_TransportManagerTestWithCorrectDeviceAdapter" ${LIBRARIES})
add_test(test_TransportManagerTestWithCorrectDeviceAdapter test_TransportManagerTestWithCorrectDeviceAdapter)

add_executable("test_TransportManagerTestWithIncorrectDeviceAdapter" ${SOURCES_TEST_WITH_INCORRECT_DEVICE_ADAPTER})
target_link_libraries("test_TransportManagerTestWithIncorrectDeviceAdapter" ${LIBRARIES})
add_test(test_TransportManagerTestWithIncorrectDeviceAdapter test_TransportManagerTestWithIncorrectDeviceAdapter)

add_executable("test_TransportManagerTestWithMultipleDeviceAdapters" ${SOURCES_TEST_WITH_MULTIPLE_DEVICE_ADAPTERS})
target_link_libraries("test_TransportManagerTestWithMultipleDeviceAdapters" ${LIBRARIES})
add_test(test_TransportManagerTestWithMultipleDeviceAdapters test_TransportManagerTestWithMultipleDeviceAdapters)

add_executable("test_TransportManagerTestWithHighDataTransferRate" ${SOURCES_TEST_WITH_HIGH_DATA_TRANSFER_RATE})
target_link_libraries("test_TransportManagerTestWithHighDataTransferRate" ${LIBRARIES})
add_test(test_TransportManagerTestWithHighDataTransferRate test_TransportManagerTestWithHighDataTransferRate)

add_executable("test_TransportManagerTestWithMultiThreadedClient" ${SOURCES_TEST_WITH_MULTI_THREADED_CLIENT})
target_link_libraries("test_TransportManagerTestWithMultiThreadedClient" ${LIBRARIES})
add_test(test_TransportManagerTestWithMultiThreadedClient test_TransportManagerTestWithMultiThreadedClient)

add_executable("test_TransportManagerTestWithMultipleClients" ${SOURCES_TEST_WITH_MULTIPLE_CLIENTS})
target_link_libraries("test_TransportManagerTestWithMultipleClients" ${LIBRARIES})
add_test(test_TransportManagerTestWithMultipleClients test_TransportManagerTestWithMultipleClients)


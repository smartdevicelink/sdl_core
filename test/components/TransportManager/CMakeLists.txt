include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/TransportManager/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
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

# Libraries seeems to be the same for all Transport Manager test
set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    TransportManager
    log4cplus
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

add_executable("test_TransportManagerDraftTest" ${SOURCES_DRAFT_TEST})
target_link_libraries("test_TransportManagerDraftTest" ${LIBRARIES})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cplus.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable("test_TransportManagerTestWithCorrectDeviceAdapter" ${SOURCES_TEST_WITH_CORRECT_DEVICE_ADAPTER})
target_link_libraries("test_TransportManagerTestWithCorrectDeviceAdapter" ${LIBRARIES})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/log4cplus.properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_test(test_TransportManagerTestWithCorrectDeviceAdapter test_TransportManagerTestWithCorrectDeviceAdapter)
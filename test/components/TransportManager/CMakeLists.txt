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
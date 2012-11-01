include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/TransportManager/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/thirdPartyLibs/logger/log4cplus-1.1.0/include/
  ${CMAKE_BINARY_DIR}/src/thirdPartyLibs/logger/log4cplus-1.1.0/include
)

set (SOURCES
    ./ListenersTest.cpp
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    TransportManager
    log4cplus
    bluetooth
)

add_executable("test_TransportManagerListeners" ${SOURCES})
target_link_libraries("test_TransportManagerListeners" ${LIBRARIES})
add_test(test_TransportManagerListeners test_TransportManagerListeners)
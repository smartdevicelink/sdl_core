include_directories (
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ${CMAKE_SOURCE_DIR}/src/components/TransportManager/include
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/logger/include/
  ${CMAKE_SOURCE_DIR}/src/components/utils/include/
)

if(BUILD_ONLY_TM)
set (SOURCES
  ./transport_manager_instance_test.cc
)

add_library("test_TransportManagerTest" ${SOURCES})
endif()

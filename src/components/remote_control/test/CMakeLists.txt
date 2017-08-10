include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/functional_module/include/
  ${CMAKE_SOURCE_DIR}/src/components/functional_module/test/
  ${CMAKE_SOURCE_DIR}/src/components/remote_control/include/
  ${CMAKE_SOURCE_DIR}/src/components/remote_control/test/include/
  ${CMAKE_SOURCE_DIR}/src/components/include/
  ${CMAKE_SOURCE_DIR}/src/components/application_manager/test/include/
  ${CMAKE_SOURCE_DIR}/src/components/connection_handler/include/
  ${CMAKE_SOURCE_DIR}/src/components/smart_objects/include/
  ${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/usage_statistics/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
  include/
)

set (SOURCES
  src/rc_module_test.cc
  src/rc_app_extension_test.cc
  src/resource_allocation_manager_impl_test.cc
  ${CMAKE_SOURCE_DIR}/src/components/application_manager/test/mock_message_helper.cc
)

set (LIBRARIES
  gtest
  gmock
  gmock_main
  RemoteControlModule
  SmartObjects
  gcov
  Policy
)

if (ENABLE_TEST_COV_COUNT)
  set(GCOV_FLAGS "-ftest-coverage -fprofile-arcs")
else()
  set(GCOV_FLAGS "")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCOV_FLAGS}")

configure_file(lc.awk lc.awk COPYONLY)
configure_file(run_calc_comments.sh run_calc_comments.sh COPYONLY)
configure_file(run_mem_check.sh run_mem_check.sh COPYONLY)
configure_file(smartDeviceLink.ini smartDeviceLink.ini COPYONLY)
configure_file(../InteriorVehicleDataCapabilities.json InteriorVehicleDataCapabilities.json COPYONLY)

# use, i.e. don't skip the full RPATH for the build tree
#SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
#SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

#SET(RPATH_DIRECTORIES
#  ${CMAKE_BINARY_DIR}/src/components/remote_control/
#  /usr/local/lib
#  /usr/local
#  ${CMAKE_BINARY_DIR}/src/components/utils
#)

#SET(CMAKE_INSTALL_RPATH "${RPATH_DIRECTORIES}")

add_custom_command(
OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libPolicy.so
                     COMMAND ${CMAKE_COMMAND} -E
                     copy  ${COMPONENTS_DIR}/policy/libPolicy.so ${CMAKE_CURRENT_BINARY_DIR})
  set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=${CMAKE_CURRENT_BINARY_DIR}")

create_test("remote_control_test" "${SOURCES}" "${LIBRARIES}")
add_subdirectory(commands)

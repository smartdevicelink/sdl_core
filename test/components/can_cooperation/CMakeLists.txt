include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/functional_module/include/
  ../../../src/components/can_cooperation/include/
  ../../../src/components/include/
  ../../../src/components/application_manager/include
  ../../../src/components/connection_handler/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    CANCooperation
    jsoncpp
    FunctionalModule
    gcov
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

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

SET(CMAKE_INSTALL_RPATH "${CMAKE_BINARY_DIR}/src/components/can_cooperation/")


create_test("test_can_module" "./src/can_module_test.cc" "${LIBRARIES}")
create_test("test_can_app_extension" "./src/can_app_extension_test.cc" "${LIBRARIES}")
create_test("test_can_tcp_connection" "./src/can_tcp_connection_test.cc" "${LIBRARIES}")
create_test("test_can_library" "./src/can_library_test.cc" "${LIBRARIES}")
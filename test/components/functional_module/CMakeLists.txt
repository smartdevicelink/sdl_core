include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/functional_module/include/
  ../../../src/components/include/
  ../../../src/components/application_manager/include
  ../../../src/components/connection_handler/include
  ../../../src/components/utils/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    Utils
    FunctionalModule
    dl
    ApplicationManager
    jsoncpp
)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_BINARY_DIR}/plugins")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
#SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


add_subdirectory(plugins)

create_test("test_generic_module" "./src/generic_module_test.cc" "${LIBRARIES}")
create_test("test_plugin_manager" "./src/plugin_manager_test.cc" "${LIBRARIES}")
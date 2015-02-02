include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/functional_module/include/
  ${CMAKE_SOURCE_DIR}/src/components/include/
  ${CMAKE_SOURCE_DIR}/src/components/application_manager/include
  ${CMAKE_SOURCE_DIR}/src/components/connection_handler/include
  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
  include
  plugins
)

set (LIBRARIES
  gtest
  gtest_main
  gmock
  gmock_main
  FunctionalModule
  dl
  ApplicationManager
  jsoncpp
  Utils
)

set(SOURCES
  ./src/generic_module_test.cc
  ./src/plugin_manager_test.cc
)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 

#SET(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_BINARY_DIR}/plugins")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
#SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_subdirectory(plugins)

create_test("function_module_test" "${SOURCES}" "${LIBRARIES}")
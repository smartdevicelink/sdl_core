include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/functional_module/include/
  ${CMAKE_SOURCE_DIR}/src/components/include/
  ${CMAKE_SOURCE_DIR}/src/components/application_manager/include
  ${CMAKE_SOURCE_DIR}/src/components/connection_handler/include
  ${CMAKE_SOURCE_DIR}/src/components/utils/include
  ${CMAKE_SOURCE_DIR}/src/components/policy/include
  ${CMAKE_SOURCE_DIR}/src/components/smart_objects/include
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
  include
  plugins
)

set (LIBRARIES
  gtest
  gmock
  gmock_main
  FunctionalModule
  dl
  ApplicationManager
  jsoncpp
  Utils
  ConfigProfile
  gcov
)

set(SOURCES
  ./src/generic_module_test.cc
  ./src/plugin_manager_test.cc
  ./src/module_timer_test.cc
)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

SET(RPATH_DIRECTORIES
  /usr/local/lib
  /usr/local
  ${CMAKE_BINARY_DIR}/src/components/utils
)

SET(CMAKE_INSTALL_RPATH "${RPATH_DIRECTORIES}")

add_subdirectory(plugins)

create_test("function_module_test" "${SOURCES}" "${LIBRARIES}")

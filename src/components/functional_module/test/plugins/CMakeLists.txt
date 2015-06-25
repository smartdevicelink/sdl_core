set(target PluginMock)

include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/components/functional_module/include/
  ${CMAKE_SOURCE_DIR}/src/components/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
)

set(SOURCES
  ./mock_generic_module.cc
)

add_library(${target} SHARED ${SOURCES})
target_link_libraries(${target})
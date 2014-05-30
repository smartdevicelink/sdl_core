include_directories (
  ./include
  ../utils/include/
  ../config_profile/include/
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
  ./src/last_state.cc
)

add_library("Resumption" ${SOURCES})

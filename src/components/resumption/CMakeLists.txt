include_directories (
  ./include
  ../utils/include/
  ../config_profile/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
  ./src/last_state.cc
)

add_library("Resumption" ${SOURCES})

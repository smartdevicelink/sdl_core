include_directories (
  ./include
  ../utils/include/
  ${CMAKE_BINARY_DIR}/src/components/  
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
)

set (SOURCES
  ./src/last_state.cc
)

add_library("Resumption" ${SOURCES})

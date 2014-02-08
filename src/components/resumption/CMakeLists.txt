include_directories (
  ./include
  ../utils/include
)

set (SOURCES
    ./src/last_state.cc
)

add_library("Resumption" ${SOURCES})

include_directories (
  ./include
)

set (SOURCES
    ./src/last_state.cc
)

add_library("Resumption" ${SOURCES})

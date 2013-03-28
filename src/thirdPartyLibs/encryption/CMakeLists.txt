include_directories (
  ./include
)

set (SOURCES
    ./src/Base64.cpp
)

add_library("encryption" ${SOURCES})

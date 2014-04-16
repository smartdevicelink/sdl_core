include_directories (
  ./include
)

set (SOURCES
    ./src/Base64.cpp
    ./src/hashing.cc
)

set (LIBRARIES crypto)

add_library("encryption" ${SOURCES})
target_link_libraries("encryption" "${LIBRARIES}")

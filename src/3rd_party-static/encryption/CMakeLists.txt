set(ENCRYPTION_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ENCRYPTION_INCLUDE_DIRECTORY ${ENCRYPTION_INCLUDE_DIRECTORY} PARENT_SCOPE)

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

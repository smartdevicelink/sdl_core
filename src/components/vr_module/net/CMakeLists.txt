set(NET_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES
  ${NET_DIR}/src/*
)

set(LIBRARIES
)

include_directories(
  ${NET_DIR}/..
  ${CMAKE_SOURCE_DIR}/src/components/utils
)

add_library("Net" ${SOURCES})

target_link_libraries("Net" ${LIBRARIES})

include_directories (
  ./include
  ../utils/include/
)

set (SOURCES
)

add_library("TransportManager" ${SOURCES})
target_link_libraries ("TransportManager" log4cxx)

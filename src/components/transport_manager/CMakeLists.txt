include_directories (
  ./include
  ../utils/include/
)

set (SOURCES
	./src/transport_manager.cc
)

add_library("TransportManager" ${SOURCES})
target_link_libraries ("TransportManager" log4cxx)

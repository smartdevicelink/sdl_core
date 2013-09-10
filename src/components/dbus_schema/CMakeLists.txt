include_directories (
  ./include/
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_BINARY_DIR}/
)

set (SOURCES
  ./src/schema.cc
  ./src/ford_message_descriptions.cc
)

add_library("DBusSchema" ${SOURCES})
target_link_libraries ("DBusSchema")

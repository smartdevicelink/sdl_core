include_directories (
  ./include/
)

set (SOURCES
  ./src/json_value.cpp
  ./src/json_writer.cpp
  ./src/json_reader.cpp
)

add_library("jsoncpp" ${SOURCES})
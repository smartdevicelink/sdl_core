include_directories (
  ./include/
)

set (SOURCES
  ./src/lib_json/json_value.cpp
  ./src/lib_json/json_writer.cpp
  ./src/lib_json/json_reader.cpp
)

add_library("jsoncpp" ${SOURCES})
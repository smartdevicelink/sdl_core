include_directories (
  ./include/
)

set (SOURCES
  ./src/json_value.cpp
  ./src/json_writer.cpp
  ./src/json_reader.cpp
)

add_library("jsoncpp" ${SOURCES})

if(CMAKE_COMPILER_IS_GNUCXX)
  SET_SOURCE_FILES_PROPERTIES(
    ./src/json_value.cpp
    ./src/json_writer.cpp
    ./src/json_reader.cpp
    PROPERTIES
    CMAKE_CXX_FLAGS "-O0 -g3 -ggdb3"
    CMAKE_EXE_LINKER_FLAGS "-m"
  )
endif()
include_directories (
  ./include
  ../utils/include/
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ./src/profile.cc
    ./src/ini_file.cc
)

add_library("ConfigProfile" ${SOURCES})
target_link_libraries("ConfigProfile" Utils)

include_directories (
  ./include
  ../utils/include/
  ../media_manager/include/
  ../protocol_handler/include
)

set (SOURCES
    ./src/profile.cc
    ./src/ini_file.cc
)

add_library("ConfigProfile" ${SOURCES})
target_link_libraries ("ConfigProfile" Utils)

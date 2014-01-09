execute_process(
  COMMAND ${CMAKE_SOURCE_DIR}/FindQt.sh -v ${qt_version} Qt5CoreConfig.cmake
  OUTPUT_VARIABLE config_file
)

if(config_file STREQUAL "")
  message(FATAL_ERROR "Qt5 Core module not found")
endif(config_file STREQUAL "")

include(${config_file})

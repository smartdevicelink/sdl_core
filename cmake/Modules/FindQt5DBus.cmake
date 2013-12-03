execute_process(
  COMMAND ${CMAKE_SOURCE_DIR}/FindQt5.sh file Qt5DBusConfig.cmake
  OUTPUT_VARIABLE config_file
)

if(config_file STREQUAL "")
  message(FATAL_ERROR "Qt5 DBus module not found")
endif(config_file STREQUAL "")

include(${config_file})

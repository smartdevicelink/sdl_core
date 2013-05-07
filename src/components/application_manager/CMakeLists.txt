include_directories (
  ./include/
  ../Utils/include/
  ../protocol_handler/include/
  ../SmartObjects/include/
  ${CMAKE_BINARY_DIR}/src/components/Interfaces/
)

set (SOURCES
./src/message.cc
./src/application_core.cc
./src/api_version_converter.cc
./src/application_impl.cc
)


add_library("ApplicationManager"
  ${SOURCES}
)

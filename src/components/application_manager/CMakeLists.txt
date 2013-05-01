include_directories (
  ./include/
  ../Utils/include/
  ../protocol_handler/include/
  ../SmartObjects/include/
)

set (SOURCES
./src/message.cc
./src/application_core.cc
)


add_library("ApplicationManager"
  ${SOURCES}
)

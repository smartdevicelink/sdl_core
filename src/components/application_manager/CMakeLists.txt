include_directories (
  ./include/
  ../Utils/include/
  ../protocol_handler/include/
)

set (SOURCES
./src/message.cc
)


add_library("ApplicationManager"
  ${SOURCES}
)

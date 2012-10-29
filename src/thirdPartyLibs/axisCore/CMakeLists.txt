include_directories (
  ./
  ./src
  ./include/
  ./utils/misc
  ./transport/bt
  ../logger/include
  ../logger/log4cplus-1.1.0/include
)

set (SOURCES
  ./src/BluetoothReader.cpp
  ./src/BluetoothWriter.cpp
  ./src/Message.cpp
  ./src/ProtocolHandler.cpp
  ./src/MessageGenerator/CMessage.cpp
)

add_library("ProtocolHandler" ${SOURCES})
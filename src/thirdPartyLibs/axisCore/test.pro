TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += src/main.cpp \
    src/ProtocolHandler.cpp \
    src/Message.cpp \
    src/BluetoothReader.cpp \
    src/BluetoothWriter.cpp \
    src/MessageGenerator/MessageGenerator.cpp \
    src/MessageGenerator/CMessage.cpp \
    src/BluetoothHandler.cpp

HEADERS += \
    include/ProtocolPacketHeader.hpp \
    include/ProtocolHandler.hpp \
    src/Message.hpp \
    include/IProtocolObserver.hpp \
    utils/misc/Types.hpp \
    src/BluetoothReader.hpp \
    src/BluetoothWriter.hpp \
    transport/bt/IBluetoothHandler.hpp \
    transport/bt/BluetoothTypes.hpp \
    transport/bt/BluetoothAPI.hpp \
    transport/bt/Blob.hpp \
    src/MessageGenerator/CMessage.hpp \
    src/BluetoothHandler.hpp

INCLUDEPATH += \
    ./ \
    ./include \
    ./src

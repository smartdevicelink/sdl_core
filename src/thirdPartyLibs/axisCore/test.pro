TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += src/main.cpp \
    src/ProtocolHandler.cpp \
    src/Message.cpp \
    src/BluetoothReader.cpp \
    src/BluetoothWriter.cpp

HEADERS += \
    include/ProtocolPacketHeader.hpp \
    include/ProtocolHandler.hpp \
    src/Message.hpp \
    include/IProtocolObserver.hpp \
    utils/misc/Types.hpp \
    src/BluetoothReader.hpp \
    src/BluetoothWriter.hpp

INCLUDEPATH += \
    ./ \
    ./include \
    ./src

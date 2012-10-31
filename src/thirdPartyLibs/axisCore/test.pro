TEMPLATE = app
CONFIG += console
CONFIG -= qt

LIBS += -lbluetooth
LIBS += -L"../logger/bin" -llog4cplus
LIBS += -lpthread

SOURCES += src/main.cpp \
    src/ProtocolHandler.cpp \
    src/Message.cpp \
    src/BluetoothReader.cpp \
    src/BluetoothWriter.cpp \
    src/MessageGenerator/CMessage.cpp \
    ../../appMain/CBTAdapter.cpp

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
    src/MessageGenerator/CMessage.hpp

INCLUDEPATH += \
    ./ \
    ./include \
    ./src \
    ./utils/misc \
    ./transport/bt \
    ../../appMain \
    ../logger/include \
    ../logger/log4cplus-1.0.4/include

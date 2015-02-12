TEMPLATE = app

QT += qml quick widgets websockets

# Adds the CANLib project path to the header file include lookup path
INCLUDEPATH += $$PWD/../CANlib

SOURCES += main.cpp \
    loger.cpp

HEADERS += loger.h

RESOURCES += qml.qrc

CONFIG += declarative_debug

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

unix:LIBS += -L$$OUT_PWD/../CANlib -lCANlib

# Default rules for deployment.
include(deployment.pri)


#-------------------------------------------------
#
# Project created by QtCreator 2015-02-10T14:14:25
#
#-------------------------------------------------

QT       -= gui
QT       += qml quick widgets websockets

TARGET = CANlib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += canlib.cpp \
    server.cpp \
    FileSystem.cpp \
    webServer.cpp \
    messageHandler.cpp \

HEADERS += canlib.h \
    server.h \
    FileSystem.h \
    color.h \
    webServer.h \
    messageHandler.h \

unix {
    target.path = /usr/lib
    INSTALLS += target
}

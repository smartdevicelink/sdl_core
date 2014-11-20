TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    server.cpp \
    FileSystem.cpp \
    loger.cpp

RESOURCES += qml.qrc

CONFIG += declarative_debug

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    server.h \
    FileSystem.h \
    loger.h \
    color.h

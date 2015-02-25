#-------------------------------------------------
#
# Project created by QtCreator 2015-02-05T16:28:32
#
#-------------------------------------------------

# Adds the CANLib project path to the header file include lookup path
INCLUDEPATH += $$PWD/../CANlib

QT       += core testlib network websockets

QT       -= gui

TARGET = CAN_Test
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase

TEMPLATE = app

unix:LIBS += -L$$OUT_PWD/../CANlib -lCANlib

SOURCES += main.cpp \
    canwstest.cpp \
    cantcptest.cpp \
    canfstest.cpp

HEADERS += \
    canwstest.h \
    cantcptest.h \
    canfstest.h

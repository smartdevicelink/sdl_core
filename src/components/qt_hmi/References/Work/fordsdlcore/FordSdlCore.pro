TEMPLATE = lib
TARGET = FordSdlCore
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.ford.sdlcore

# Input
SOURCES += \
    fordsdlcore_plugin.cpp \
    smartdevicelink.cpp \
    sdlalert.cpp \
    QtWebsocket/QWsSocket.cpp \
    QtWebsocket/QWsServer.cpp \
    sdlappslistmodel.cpp

HEADERS += \
    fordsdlcore_plugin.h \
    smartdevicelink.h \
    sdlalert.h \
    QtWebsocket/QWsSocket.h \
    QtWebsocket/QWsServer.h \
    sdlappslistmodel.h

OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = ../BananaSnacks/plugins/com/ford/sdlcore
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

linux-g++* {
    QMAKE_CXXFLAGS += -m32
    QMAKE_LFLAGS += -m32
}
QMAKE_LIBDIR += /usr/lib/i386-linux-gnu/mesa

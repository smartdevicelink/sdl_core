TEMPLATE = lib
TARGET = hmi_dbus
QT += qml quick dbus
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = sdl.core.interface

DBUS_INTERFACES += notify.xml

# Input
SOURCES += \
    dbus_plugin.cpp \
    qt_dbus.cpp \
    sdlproxy.cpp \
    hmiproxy.cpp

HEADERS += \
    dbus_plugin.h \
    qt_dbus.h \
    optional_argument.h \
    qml_dbus_common.h \
    sdlproxy.h \
    hmiproxy.h

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
#    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    installPath = ../../qml_model/sdl/core/api
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

QMAKE_LIBDIR += /usr/lib/i386-linux-gnu/mesa

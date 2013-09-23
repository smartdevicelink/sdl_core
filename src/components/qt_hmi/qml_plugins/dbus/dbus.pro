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
    api.cpp \
    presetBankCapabilities.cpp \
    buttonCapabilities.cpp \
    buttonsadaptor.cpp

HEADERS += \
    dbus_plugin.h \
    api.h \
    presetBankCapabilities.h \
    buttonCapabilities.h \
    buttonsadaptor.h \
    optionalArgument.h

OTHER_FILES = qmldir \
    api.xml

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

QMAKE_POST_LINK = make install

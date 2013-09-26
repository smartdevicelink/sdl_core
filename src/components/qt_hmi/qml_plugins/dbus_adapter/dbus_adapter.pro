TEMPLATE = lib
TARGET = DbusAdapter
QT += qml quick dbus
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.ford.sdl.hmi.DbusAdapter

DBUS_INTERFACES += notify.xml

# Input
SOURCES += \
    dbus_plugin.cpp \
    api.cpp \
    button_capabilities.cpp \
    buttons_adaptor.cpp \
    preset_bank_capabilities.cpp

HEADERS += \
    dbus_plugin.h \
    api.h \
    button_capabilities.h \
    buttons_adaptor.h \
    optional_argument.h \
    preset_bank_capabilities.h

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
    installPath = ../../qml_model/com/ford/sdl/hmi/dbus_adapter
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

QMAKE_LIBDIR += /usr/lib/i386-linux-gnu/mesa

QMAKE_POST_LINK = make install

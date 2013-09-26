TEMPLATE = lib
TARGET = HWButtons
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.ford.sdl.hmi.HWButtons

# Input
SOURCES += \
    hmi_plugin.cpp \
    masked_container.cpp \
    attributed_mouse_event.cpp

HEADERS += \
    attributed_mouse_event.h \
    masked_container.h \
    hmi_plugin.h

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
    installPath = ../../qml_model/com/ford/sdl/hmi/hw_buttons
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
QMAKE_LIBDIR += /usr/lib/i386-linux-gnu/mesa

QMAKE_POST_LINK = make install

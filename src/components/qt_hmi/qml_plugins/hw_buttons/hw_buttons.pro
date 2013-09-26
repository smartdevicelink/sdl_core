# \file hw_buttons.pro
# \brief Drow masked mouse area.
# Copyright (c) 2013, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

TEMPLATE = lib
TARGET = HWButtons
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.ford.sdl.hmi.HWButtons

# Input
SOURCES += \
    masked_container.cpp \
    attributed_mouse_event.cpp \
    hmi_hwbuttons.cpp

HEADERS += \
    attributed_mouse_event.h \
    masked_container.h \
    hmi_hwbuttons.h

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

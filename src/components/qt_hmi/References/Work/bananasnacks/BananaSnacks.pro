# Add more folders to ship with the application, here
folder_01.source = qml/Bananas
folder_01.target = qml

folder_02.source = plugins/com
folder_02.target = plugins
DEPLOYMENTFOLDERS = folder_01 folder_02

SOURCES += main.cpp

linux-g++* {
    QMAKE_CXXFLAGS += -m32
    QMAKE_LFLAGS += -m32 -L/usr/lib/i386-linux-gnu/mesa
}

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qml/Bananas/Layouts/TitledLayout.qml

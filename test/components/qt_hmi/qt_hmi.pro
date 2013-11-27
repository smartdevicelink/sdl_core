# Add more folders to ship with the application, here
folder_01.source = src
folder_01.target = qml

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =   src/ ../../../src/components/qt_hmi/qml_model

QT += qml
# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += qt_hmi_test.cc

CONFIG += warn_on qmltestcase
OTHER_FILES += src/*.qml

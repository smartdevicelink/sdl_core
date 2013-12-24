How to view QML HMI.

1) Run setup_env.sh from root folder of SDL project (this will install right version of Qt and CMake)
2) Run CMake with option  HMI2=ON  “cmake  -DHMI2=ON <root_SDL_folder>” (this will build core+dbus adapter and QML HMI+ dbus adapter)
3) Run “<QT510_INSTALL_PATH>/qmlscene <root_SDL_folder>/src/components/qt_hmi/qml_model_qt5/MainWindow.qml”
4) Run SDL
 

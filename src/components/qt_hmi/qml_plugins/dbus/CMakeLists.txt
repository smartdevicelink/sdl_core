cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../../../../../cmake/Modules") # to be removed

FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5DBus REQUIRED)
FIND_PACKAGE(Qt5Qml REQUIRED)
FIND_PACKAGE(Qt5Quick REQUIRED)

set(CMAKE_AUTOMOC ON)

set(SOURCES
  dbus_plugin.cpp
  api.cpp
  presetBankCapabilities.cpp
  buttonCapabilities.cpp
  buttonsadaptor.cpp
)

set(target hmi_dbus)

add_library(${target} SHARED ${SOURCES})

qt5_use_modules(${target} Core DBus Qml Quick)

set(destination ${CMAKE_CURRENT_SOURCE_DIR}/../../qml_model/sdl/core/api)

install(TARGETS ${target}
  DESTINATION ${destination}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)
install(FILES qmldir DESTINATION ${destination})

add_custom_command(TARGET ${target} POST_BUILD COMMAND make install)

cmake_minimum_required(VERSION 2.8.9)

FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Qml REQUIRED)
FIND_PACKAGE(Qt5Quick REQUIRED)

set(CMAKE_AUTOMOC ON)

set(SOURCES
  attributed_mouse_event.cc
  hmi_hwbuttons.cc
  masked_container.cc
)

set(target HWButtons)

add_library(${target} SHARED ${SOURCES})

qt5_use_modules(${target} Core Qml Quick)

set(destination ${CMAKE_CURRENT_SOURCE_DIR}/../../qml_model/com/ford/sdl/hmi/hw_buttons)

install(TARGETS ${target}
  DESTINATION ${destination}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)
install(FILES qmldir DESTINATION ${destination})

add_custom_command(TARGET ${target} POST_BUILD COMMAND make install)

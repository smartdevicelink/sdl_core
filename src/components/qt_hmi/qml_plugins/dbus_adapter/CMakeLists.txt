cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_AUTOMOC ON)

set(components_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set(dbus_dir ${components_dir}/dbus)
set(codegen_dir ${dbus_dir}/codegen)
set(interfaces_dir ${components_dir}/interfaces)
add_custom_command(OUTPUT qml_dbus.cc COMMAND python ${codegen_dir}/make_qml_dbus_cpp.py --infile ${interfaces_dir}/HMI_API.xml)

set(SOURCES
  dbus_plugin.cc
  qml_dbus.cc
  sdlproxy.cc
  hmiproxy.cc
)

set(target DBusAdapter)

add_library(${target} SHARED ${SOURCES})

qt5_use_modules(${target} Core DBus Qml Quick)

set(destination ${CMAKE_CURRENT_SOURCE_DIR}/../../qml_model/com/ford/sdl/hmi/dbus_adapter)

install(TARGETS ${target}
  DESTINATION ${destination}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)
install(FILES qmldir DESTINATION ${destination})

add_custom_command(TARGET ${target} POST_BUILD COMMAND make install)

message("Looking for D-Bus in ${DBUS_BUILD_DIRECTORY}")

find_path(DBUS_INCLUDE_DIR
  NAMES dbus/dbus.h
  PATHS /usr/lib/dbus-1.0/include
      /usr/include/dbus-1.0)

set(DBUS_INCLUDE_DIRS ${DBUS_INCLUDE_DIR} ${DBUS_ARCH_INCLUDE_DIR})

mark_as_advanced(DBUS_INCLUDE_DIR)

if (DBUS_INCLUDE_DIRS)
  set(DBUS_FOUND true)
endif()

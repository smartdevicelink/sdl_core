message("Looking for D-Bus")

find_path(DBUS_INCLUDE_DIR
	NAMES dbus/dbus.h
	PATHS /usr/lib/dbus-1.0/include
	      /usr/include/dbus-1.0)

find_path(DBUS_ARCH_INCLUDE_DIR
	NAMES dbus/dbus-arch-deps.h
	PATHS /usr/lib/dbus-1.0/include
	      /usr/include/dbus-1.0
	      /usr/lib/x86_64-linux-gnu/dbus-1.0/include
	      /usr/lib/i386-linux-gnu/dbus-1.0/include)

set(DBUS_INCLUDE_DIRS ${DBUS_INCLUDE_DIR} ${DBUS_ARCH_INCLUDE_DIR})

mark_as_advanced(DBUS_INCLUDE_DIR)
mark_as_advanced(DBUS_ARCH_INCLUDE_DIR)

if (DBUS_INCLUDE_DIRS)
	set(DBUS_FOUND true)
endif()

set(LIBUSB_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBUSB_INCLUDE_DIRECTORY ${LIBUSB_INCLUDE_DIRECTORY} PARENT_SCOPE)

set(LIBUSB_LIBS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(LIBUSB_LIBS_DIRECTORY ${LIBUSB_LIBS_DIRECTORY} PARENT_SCOPE)

include_directories (
  ./libusb/os/
  ./libusb/
  ./
)

set (LIBUSB_SOURCES
	./libusb/core.c
	./libusb/descriptor.c
	./libusb/hotplug.c
	./libusb/io.c
	./libusb/strerror.c
	./libusb/sync.c
	./libusb/os/linux_udev.c
	./libusb/os/linux_netlink.c
	./libusb/os/linux_usbfs.c
	./libusb/os/poll_posix.c
	./libusb/os/threads_posix.c
)

add_library("Libusb-1.0.16" ${LIBUSB_SOURCES})


target_link_libraries("Libusb-1.0.16" udev)

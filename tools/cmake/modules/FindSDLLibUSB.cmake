#  - Try to find libusb
#
#  LIBUSB_INCLUDE_DIRECTORY - the libusb include directory
#  LIBUSB_LIBRARIES - libusb libraries to link

get_os(OS)
if(${OS} STREQUAL "posix")
  set(LIBUSB_INCLUDE_DIRECTORY /usr/include/libusb-1.0)
  set(LIBUSB_LIBRARIES usb-1.0 udev)
elseif(${OS} STREQUAL "win")
  get_arch(ARCH)
  if(${ARCH} STREQUAL "x86")
    set(LIBUSB_DIR ${CMAKE_SOURCE_DIR}/build/libusbx_win_x86)
  elseif(${ARCH} STREQUAL "x64")
    set(LIBUSB_DIR ${CMAKE_SOURCE_DIR}/build/libusbx_win_x64)
  endif()
  set(LIBUSB_INCLUDE_DIRECTORY ${LIBUSB_DIR}/libusb)
  set(LIBUSB_LIBRARIES ${LIBUSB_DIR}/bin/libusb-1.0.lib)
endif()

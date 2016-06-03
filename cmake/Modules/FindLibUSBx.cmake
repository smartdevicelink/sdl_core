#  - Try to find libusbx
#
#  LIBUSBX_INCLUDE_DIRECTORY - the libusbx include directory
#  LIBUSBX_LIBS_DIRECTORY - the libusbx libs directory
#  LIBUSBX_LIBRARIES - libusbx libraries to link

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if (QT_PORT)
    set(LIBUSBX_DIR ${CMAKE_SOURCE_DIR}/build/libusbx_win_x86)
  else()
    set(LIBUSBX_DIR ${CMAKE_SOURCE_DIR}/build/libusbx_win_x64)
  endif()

  set(LIBUSBX_INCLUDE_DIRECTORY ${LIBUSBX_DIR}/libusb)
  set(LIBUSBX_LIBS_DIRECTORY ${LIBUSBX_DIR}/bin)
  set(LIBUSBX_LIBRARIES ${LIBUSBX_LIBS_DIRECTORY}/libusb-1.0.lib)
else()
  set(LIBUSBX_INCLUDE_DIRECTORY /usr/local/include/libusb-1.0)
  set(LIBUSBX_LIBS_DIRECTORY /usr/local/lib)
  set(LIBUSBX_LIBRARIES ${LIBUSBX_LIBS_DIRECTORY}/libusb-1.0.a udev)
endif()

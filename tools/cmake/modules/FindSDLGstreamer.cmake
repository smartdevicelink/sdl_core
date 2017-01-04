#  - Try to find Gstreamer
#
#  GSTREAMER_INCLUDE_DIRECTORY - the Gstreamer include directory
#  GSTREAMER_LIBRARIES - the Gstreamer libraries

get_os(OS)
if(${OS} STREQUAL "posix")
  find_package(Gstreamer-1.0 REQUIRED)
  find_package(Glib-2.0 REQUIRED)
  find_package(PkgConfig)
  pkg_check_modules(GLIB2 REQUIRED glib-2.0)
  add_definitions(${GLIB2_CFLAGS})
  set(GSTREAMER_INCLUDE_DIRECTORY
    ${GSTREAMER_gst_INCLUDE_DIR}
    ${GLIB_glib_2_INCLUDE_DIR}
  )
  set(GSTREAMER_LIBRARIES
    ${GSTREAMER_gstreamer_LIBRARY}
    gobject-2.0
    glib-2.0
  )
elseif(${OS} STREQUAL "win")
  get_sdk(SDK)
  if(NOT SDK)
    if(EXISTS $ENV{SDL_GSTREAMER_DIR})
      set(GSTREAMER_DIR $ENV{SDL_GSTREAMER_DIR})
    else()
      message(FATAL_ERROR "Could not find GSTREAMER (Check SDL_GSTREAMER_DIR environment variable)")
    endif()
    set(GSTREAMER_INCLUDE_DIRECTORY
      ${GSTREAMER_DIR}/include/gstreamer-0.10
      ${GSTREAMER_DIR}/include/libxml2
      ${GSTREAMER_DIR}/include/glib-2.0
      ${GSTREAMER_DIR}/lib/glib-2.0/include
    )
    set(GSTREAMER_LIBRARIES
      ${GSTREAMER_DIR}/lib/gstreamer-0.10.lib
      ${GSTREAMER_DIR}/lib/glib-2.0.lib
      ${GSTREAMER_DIR}/lib/gobject-2.0.lib
    )
  endif()
endif()

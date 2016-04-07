#  - Try to find Gstreamer-0.10
#
#  GSTREAMER_INCLUDE_DIR - the Gstreamer-0.10 include directory
#  GSTREAMER_LIBRARIES - the Gstreamer-0.10 libraries

if(EXISTS $ENV{SDL_GSTREAMER_DIR})
    set(GSTREAMER_DIR $ENV{SDL_GSTREAMER_DIR})
else()
    message(FATAL_ERROR "Could not find GSTREAMER (Check SDL_GSTREAMER_DIR environment variable)")
endif()

set(GSTREAMER_INCLUDE_DIR
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

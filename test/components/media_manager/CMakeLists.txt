include_directories (
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
  ../../../src/components/media_manager/include
  ../../../src/components/utils/include/
  ../../../test/components/media_manager/include
    /usr/lib/i386-linux-gnu/glib-2.0/include
    /usr/lib/x86_64-linux-gnu/glib-2.0/include/
  ${GSTREAMER_gst_INCLUDE_DIR}
  ${GLIB_glib_2_INCLUDE_DIR}
  ${CMAKE_BINARY_DIR}/src/components/
)

if (EXTENDED_MEDIA_MODE)
  include_directories (
    ${GSTREAMER_gst_INCLUDE_DIR}
    ${GLIB_glib_2_INCLUDE_DIR}
  )
endif()

set (SOURCES
  ./src/media_manager_impl_test.cc
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    MediaManager
    ApplicationManager
    connectionHandler
    SmartObjects
    Utils
    pulse-simple
    ConfigProfile
    formatters
    jsoncpp
)

if (EXTENDED_MEDIA_MODE)
  list(APPEND LIBRARIES
    ${GSTREAMER_gstreamer_LIBRARY})
endif()

create_test("test_MediaManager" "${SOURCES}" "${LIBRARIES}")

if(ENABLE_LOG)
  target_link_libraries("test_MediaManager" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()
# vim: set ts=2 sw=2 et:

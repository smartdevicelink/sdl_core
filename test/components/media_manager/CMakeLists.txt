find_package(Glib-2.0 REQUIRED)

# required to find 'glibconfig.h'
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})

include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/media_manager/include
  ../../../src/components/utils/include/
  ../../../src/components/protocol_handler/include/
  ../../../test/components/media_manager/include
  ${CMAKE_BINARY_DIR}/src/components/
)

if (DEFAULT_MEDIA_FLAG)
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
    log4cxx
)

if (DEFAULT_MEDIA_FLAG)
  list(APPEND LIBRARIES
    ${GSTREAMER_gstreamer_LIBRARY})
endif()

create_test("test_MediaManager" "${SOURCES}" "${LIBRARIES}")

# vim: set ts=2 sw=2 et:

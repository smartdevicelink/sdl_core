find_package(Glib-2.0 REQUIRED)

# required to find 'glibconfig.h'
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})

include_directories (
  ${GSTREAMER_gst_INCLUDE_DIR}
  ${GLIB_glib_2_INCLUDE_DIR}
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/media_manager/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/components/utils/include/
  ../../../src/components/protocol_handler/include/
  ../../../test/components/media_manager/include
  ${CMAKE_BINARY_DIR}/src/components/
)

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
    ${GSTREAMER_gstreamer_LIBRARY}
    pulse-simple
    ConfigProfile
    formatters
    jsoncpp
    log4cxx
)

create_test("test_MediaManager" "${SOURCES}" "${LIBRARIES}")
#add_library("test_MediaManagerTest" "${SOURCES}")

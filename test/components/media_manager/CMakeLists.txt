include_directories (
  ${GSTREAMER_gst_INCLUDE_DIR}
  ${GLIB_glib_2_INCLUDE_DIR}
  ${GLIB_glibconfig_2_INCLUDE_DIR}
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
    ${GSTREAMER_gstreamer_LIBRARY}
)

#create_test("test_MediaManager" "${SOURCES}" "${LIBRARIES}")
add_library("test_MediaManagerTest" "${SOURCES}")

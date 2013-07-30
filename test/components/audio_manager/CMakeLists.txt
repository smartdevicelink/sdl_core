include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/audio_manager/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/components/utils/include/
  ../../../test/components/audio_manager/include
    /usr/lib/i386-linux-gnu/glib-2.0/include
  ${GSTREAMER_gst_INCLUDE_DIR}
  ${GLIB_glib_2_INCLUDE_DIR}
)

set (SOURCES
  ./src/audio_manager_impl_test.cc
)

set (LIBRARIES
    gtest
    gtest_main
    gmock
    gmock_main
    AudioManager
    gstreamer-1.0
    gobject-2.0
    gmodule-2.0
    gthread-2.0
    glib-2.0
)

create_test("test_AudioManager" "${SOURCES}" "${LIBRARIES}")
add_library("test_AudioManagerTest" "${SOURCES}")

include_directories (
  ../../../src/thirdPartyLibs/gmock-1.6.0/include
  ../../../src/thirdPartyLibs/gmock-1.6.0/gtest/include
  ../../../src/components/audio_manager/include
  ../../../src/thirdPartyLibs/logger/include/
  ../../../src/components/utils/include/
  ../../../test/components/audio_manager/include
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
)

create_test("test_AudioManager" "${SOURCES}" "${LIBRARIES}")

add_library("test_AudioManagerTest" "${SOURCES}")

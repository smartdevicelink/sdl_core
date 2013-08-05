find_package(Gstreamer-1.0 REQUIRED)
find_package(Glib-2.0 REQUIRED)

include_directories (
  ./include
  ../utils/include/
  ../../thirdPartyLibs/logger/include/
  ../../thirdPartyLibs/MessageBroker/include/
  /usr/lib/i386-linux-gnu/glib-2.0/include
  /usr/lib/x86_64-linux-gnu/glib-2.0/include/
  ${GSTREAMER_gst_INCLUDE_DIR}
  ${GLIB_glib_2_INCLUDE_DIR}
)

set(LIBRARIES
  ${GSTREAMER_gstreamer_LIBRARY}
)

set (SOURCES
    ./src/audio_manager_impl.cc
    ./src/from_mic_to_file_recorder_thread.cc
    ./src/a2dp_source_player_thread.cc
)

add_library("AudioManager" ${SOURCES} )
target_link_libraries ("AudioManager" Utils)
target_link_libraries ("AudioManager" log4cxx)
target_link_libraries ("AudioManager" pulse-simple)
target_link_libraries ("AudioManager" ${LIBRARIES})

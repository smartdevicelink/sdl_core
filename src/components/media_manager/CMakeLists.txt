if (EXTENDED_MEDIA_MODE)
find_package(Gstreamer-1.0 REQUIRED)
find_package(Glib-2.0 REQUIRED)
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})
set(default_includes
  ${GSTREAMER_gst_INCLUDE_DIR}
  ${GLIB_glib_2_INCLUDE_DIR}
)
set(default_sources
    ./src/audio/a2dp_source_player_adapter.cc
    ./src/audio/from_mic_recorder_adapter.cc
    ./src/audio/from_mic_to_file_recorder_thread.cc
    ./src/audio/socket_audio_streamer_adapter.cc
    ./src/audio/pipe_audio_streamer_adapter.cc
    ./src/video/socket_video_streamer_adapter.cc
    ./src/video/pipe_video_streamer_adapter.cc
    ./src/video/video_stream_to_file_adapter.cc
    ./src/pipe_streamer_adapter.cc
    ./src/socket_streamer_adapter.cc
)
set(LIBRARIES
  ${GSTREAMER_gstreamer_LIBRARY}
  pulse-simple
  pulse
  gobject-2.0
  glib-2.0
)
else(EXTENDED_MEDIA_MODE)
set(default_includes
)
set(default_sources
  ./src/video/video_stream_to_file_adapter.cc
  ./src/audio/socket_audio_streamer_adapter.cc
  ./src/audio/pipe_audio_streamer_adapter.cc
  ./src/video/socket_video_streamer_adapter.cc
  ./src/video/pipe_video_streamer_adapter.cc
  ./src/video/video_stream_to_file_adapter.cc
  ./src/pipe_streamer_adapter.cc
  ./src/socket_streamer_adapter.cc
)
set(LIBRARIES
  ProtocolLibrary
)
endif()

include_directories (
  ./include
  ./include/audio/
  ./include/video/
  ../utils/include/
  ../protocol_handler/include/
  ../connection_handler/include/
  ../application_manager/include/
  ../smart_objects/include/
  ../hmi_message_handler/include/
  ../formatters/include
  ../config_profile/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
  ${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/include/
  ${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/usage_statistics/include/
  ${default_includes}
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ./src/media_adapter_impl.cc
    ./src/audio/from_mic_recorder_listener.cc
    ./src/audio/audio_stream_sender_thread.cc
    ./src/streamer_listener.cc
    ./src/media_manager_impl.cc
)

add_library("MediaManager" ${SOURCES} ${default_sources})
target_link_libraries("MediaManager" ${LIBRARIES})

if(BUILD_TESTS)
  #add_subdirectory(test)
endif()

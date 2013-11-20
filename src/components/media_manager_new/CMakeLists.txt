
add_definitions(${GLIB2_CFLAGS})
add_definitions(-DDEFAULT_MEDIA)

include_directories (
  ./include
  ../utils/include/
  ../protocol_handler/include/
  ../connection_handler/include/
  ../transport_manager/include/
  ../application_manager/include/
  ../smart_objects/include/
  ../hmi_message_handler/include/
  ../mobile_message_handler/include/
  ../request_watchdog/include/
  ../formatters/include
  ../config_profile/include/
  ${GSTREAMER_gst_INCLUDE_DIR}
  ${GLIB_glib_2_INCLUDE_DIR}
  ${CMAKE_BINARY_DIR}/src/components/
)

set (SOURCES
    ./src/media_adapter_impl.cc
    ./src/a2dp_source_player_adapter.cc
    ./src/from_mic_recorder_adapter.cc
    ./src/from_mic_to_file_recorder_thread.cc
    ./src/from_mic_recorder_listener.cc
    ./src/audio_stream_sender_thread.cc
    ./src/socket_video_streamer_adapter.cc
    ./src/pipe_video_streamer_adapter.cc
    ./src/video_streamer_listener.cc
)

add_library("MediaManagerNew" ${SOURCES} )

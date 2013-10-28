find_package(Gstreamer-1.0 REQUIRED)
find_package(Glib-2.0 REQUIRED)
find_package(LibXml2 REQUIRED)

# required to find 'glibconfig.h'
find_package(PkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})

include_directories (
  ./include
  ../utils/include/
  ../protocol_handler/include/
  ../interfaces
  ../application_manager/include/
  
  ../hmi_message_handler/include
  ../mobile_message_handler/include
  ../connection_handler/include/
  ../transport_manager/include/
  ../protocol_handler/include/
  ../config_profile/include/
  ../request_watchdog/include/
  ../smart_objects/include/
  ../formatters/include/
  
    ../../thirdPartyLibs/logger/include/
  ../../thirdPartyLibs/MessageBroker/include/
  ../application_manager/include/
  ../connection_handler/include/
  ${GSTREAMER_gst_INCLUDE_DIR}
  ${GLIB_glib_2_INCLUDE_DIR}
  ${CMAKE_BINARY_DIR}/src/components/
  ${LIBXML2_INCLUDE_DIR}
)

set(LIBRARIES
  ${GSTREAMER_gstreamer_LIBRARY}
)

set (SOURCES
    ./src/media_manager_impl.cc
    ./src/from_mic_to_file_recorder_thread.cc
    ./src/a2dp_source_player_thread.cc
    ./src/video_server.cc
    ./src/audio_stream_sender_thread.cc
)

add_library("MediaManager" ${SOURCES} )
target_link_libraries ("MediaManager" Utils)
target_link_libraries ("MediaManager" log4cxx)
target_link_libraries ("MediaManager" pulse-simple)
target_link_libraries ("MediaManager" pulse)
target_link_libraries ("MediaManager" gobject-2.0)
target_link_libraries ("MediaManager" glib-2.0)
target_link_libraries ("MediaManager" ${LIBRARIES})

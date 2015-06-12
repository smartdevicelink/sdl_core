# Copyright (c) 2014, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.


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
    ${COMPONENTS_DIR}/media_manager/src/audio/a2dp_source_player_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/from_mic_recorder_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/from_mic_to_file_recorder_thread.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/socket_audio_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/pipe_audio_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/video/socket_video_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/video/pipe_video_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/video/video_stream_to_file_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/pipe_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/socket_streamer_adapter.cc
)
set(LIBRARIES
    ${GSTREAMER_gstreamer_LIBRARY}
    ApplicationManager
    pulse-simple
    pulse
    gobject-2.0
    glib-2.0
)
else(EXTENDED_MEDIA_MODE)
set(default_includes
)
set(default_sources
    ${COMPONENTS_DIR}/media_manager/src/video/video_stream_to_file_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/socket_audio_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/pipe_audio_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/video/socket_video_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/video/pipe_video_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/video/video_stream_to_file_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/pipe_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/socket_streamer_adapter.cc
)
set(LIBRARIES
    ProtocolLibrary
)
endif()

include_directories (
    include
    ${COMPONENTS_DIR}/media_manager/include/audio/
    ${COMPONENTS_DIR}/media_manager/include/video/
    ${COMPONENTS_DIR}/utils/include/
    ${COMPONENTS_DIR}/protocol_handler/include/
    ${COMPONENTS_DIR}/connection_handler/include/
    ${COMPONENTS_DIR}/application_manager/include/
    ${COMPONENTS_DIR}/smart_objects/include/
    ${COMPONENTS_DIR}/hmi_message_handler/include/
    ${COMPONENTS_DIR}/formatters/include/
    ${COMPONENTS_DIR}/config_profile/include/
    ${JSONCPP_INCLUDE_DIRECTORY}
    ${CMAKE_BINARY_DIR}/src/components/
    ${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/include/
    ${CMAKE_SOURCE_DIR}/src/components/policy/src/policy/usage_statistics/include/
    ${default_includes}
    ${LOG4CXX_INCLUDE_DIRECTORY}
)

set (SOURCES
    ${COMPONENTS_DIR}/media_manager/src/media_adapter_impl.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/from_mic_recorder_listener.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/audio_stream_sender_thread.cc
    ${COMPONENTS_DIR}/media_manager/src/streamer_listener.cc
    ${COMPONENTS_DIR}/media_manager/src/media_manager_impl.cc
)

add_library("MediaManager" ${SOURCES} ${default_sources})
target_link_libraries("MediaManager" ${LIBRARIES})

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

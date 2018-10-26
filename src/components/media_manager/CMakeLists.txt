# Copyright (c) 2016, Ford Motor Company
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

project(media_manager)

add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set(${PROJECT_NAME}_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

target_sources(${PROJECT_NAME}
  PRIVATE
    #src
    "${CMAKE_CURRENT_LIST_DIR}/src/file_streamer_adapter.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/media_adapter_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/media_manager_impl.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/pipe_streamer_adapter.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/socket_streamer_adapter.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/streamer_adapter.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/streamer_listener.cc"

    #audio
    "${CMAKE_CURRENT_LIST_DIR}/src/audio/audio_stream_sender_thread.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/audio/file_audio_streamer_adapter.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/audio/from_mic_recorder_listener.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/audio/pipe_audio_streamer_adapter.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/audio/socket_audio_streamer_adapter.cc"

    #video
    "${CMAKE_CURRENT_LIST_DIR}/src/video/file_video_streamer_adapter.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/video/pipe_video_streamer_adapter.cc"
    "${CMAKE_CURRENT_LIST_DIR}/src/video/socket_video_streamer_adapter.cc"
    #"${CMAKE_CURRENT_LIST_DIR}/src/video/video_stream_to_file_adapter.cc"
)

if(EXTENDED_MEDIA_MODE)
  target_sources(${PROJECT_NAME}
    PRIVATE
      #audio
      "${CMAKE_CURRENT_LIST_DIR}/src/audio/a2dp_source_player_adapter.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/audio/from_mic_recorder_adapter.cc"
      "${CMAKE_CURRENT_LIST_DIR}/src/audio/from_mic_to_file_recorder_thread.cc"
  )
endif()

target_link_libraries( ${PROJECT_NAME}
  PUBLIC
    utils::utils
    application_manager::application_manager
    $<$<BOOL:${SYNC_USE_MQ_SERVER}>:mq>
  PRIVATE
    components::interface
)

if(QNXNTO)
  target_link_libraries( ${PROJECT_NAME}
    PUBLIC
      socket
  )
endif()

target_include_directories( ${PROJECT_NAME}
  PUBLIC ${${PROJECT_NAME}_INCLUDE_DIRECTORY}
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
)

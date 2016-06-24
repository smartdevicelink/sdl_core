# Copyright (c) 2014-2015, Ford Motor Company
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

include_directories(
    ${COMPONENTS_DIR}/media_manager/include
    ${COMPONENTS_DIR}/utils/include/
    ${COMPONENTS_DIR}/protocol_handler/include/
    ${COMPONENTS_DIR}/connection_handler/include/
    ${COMPONENTS_DIR}/application_manager/include/
    ${COMPONENTS_DIR}/smart_objects/include/
    ${COMPONENTS_DIR}/hmi_message_handler/include/
    ${COMPONENTS_DIR}/formatters/include/
    ${COMPONENTS_DIR}/config_profile/include/
    ${COMPONENTS_DIR}/policy/include/
    ${JSONCPP_INCLUDE_DIRECTORY}
    ${CMAKE_BINARY_DIR}/src/components/
)

set(COMMON_INCLUDES
    ${COMPONENTS_DIR}/include/media_manager/media_manager.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/file_streamer_adapter.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/media_adapter.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/media_adapter_impl.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/media_adapter_listener.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/media_manager_impl.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/pipe_streamer_adapter.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/socket_streamer_adapter.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/streamer_adapter.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/streamer_listener.h
)
set(AUDIO_INCLUDES
    ${COMPONENTS_DIR}/media_manager/include/media_manager/audio/socket_audio_streamer_adapter.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/audio/pipe_audio_streamer_adapter.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/audio/file_audio_streamer_adapter.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/audio/from_mic_recorder_listener.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/audio/audio_stream_sender_thread.h
)
set(VIDEO_INCLUDES
    ${COMPONENTS_DIR}/media_manager/include/media_manager/video/socket_video_streamer_adapter.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/video/pipe_video_streamer_adapter.h
    ${COMPONENTS_DIR}/media_manager/include/media_manager/video/file_video_streamer_adapter.h
)

set(COMMON_SOURCES
    ${COMPONENTS_DIR}/media_manager/src/media_manager_impl.cc
    ${COMPONENTS_DIR}/media_manager/src/media_adapter_impl.cc
    ${COMPONENTS_DIR}/media_manager/src/streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/streamer_listener.cc
    ${COMPONENTS_DIR}/media_manager/src/pipe_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/socket_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/file_streamer_adapter.cc
)

set(AUDIO_SOURCES
    ${COMPONENTS_DIR}/media_manager/src/audio/socket_audio_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/pipe_audio_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/file_audio_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/from_mic_recorder_listener.cc
    ${COMPONENTS_DIR}/media_manager/src/audio/audio_stream_sender_thread.cc
)
set(VIDEO_SOURCES
    ${COMPONENTS_DIR}/media_manager/src/video/socket_video_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/video/pipe_video_streamer_adapter.cc
    ${COMPONENTS_DIR}/media_manager/src/video/file_video_streamer_adapter.cc
)

set(LIBRARIES
    protocol
    application_manager
)

if(LOG4CXX_LOGGER)
    include_directories(
        ${LOG4CXX_INCLUDE_DIRECTORY}
    )

    list(APPEND LIBRARIES
        log4cxx
    )
endif()

if(EXTENDED_MEDIA_MODE)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        if(WIN_NATIVE)
            find_package(WinGstreamer-0.10 REQUIRED)

            include_directories(
                ${GSTREAMER_INCLUDE_DIR}
            )

            list(APPEND LIBRARIES
                ${GSTREAMER_LIBRARIES}
            )
            list(APPEND AUDIO_SOURCES
                ${COMPONENTS_DIR}/media_manager/src/audio/from_mic_to_file_recorder_thread.cc
            )
        elseif(QT_PORT)
            find_package(Qt5 REQUIRED Multimedia)
            list(APPEND AUDIO_SOURCES
                ${COMPONENTS_DIR}/media_manager/src/audio/from_mic_to_file_recorder_thread_qt.cc
            )
            # Files copied for the Qt MOC.
            # Because MOC is looking headre nearby with *.cc
            # This headers include in .git exception. see .gitignore
            EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                  ${COMPONENTS_DIR}/media_manager/include/media_manager/audio/from_mic_to_file_recorder_thread.h
                  ${COMPONENTS_DIR}/media_manager/src/audio/from_mic_to_file_recorder_thread.h)
            SET(CMAKE_INCLUDE_CURRENT_DIR ON)
            qt5_generate_moc( from_mic_to_file_recorder_thread_qt.cc from_mic_to_file_recorder_thread_qt.moc)
        endif()
    else()
        find_package(Gstreamer-1.0 REQUIRED)
        find_package(Glib-2.0 REQUIRED)
        find_package(PkgConfig)
        pkg_check_modules(GLIB2 REQUIRED glib-2.0)
        add_definitions(${GLIB2_CFLAGS})

        include_directories(
            ${GSTREAMER_gst_INCLUDE_DIR}
            ${GLIB_glib_2_INCLUDE_DIR}
        )

        list(APPEND LIBRARIES
            ${GSTREAMER_gstreamer_LIBRARY}
            pulse-simple
            pulse
            gobject-2.0
            glib-2.0
        )
        list(APPEND AUDIO_SOURCES
            ${COMPONENTS_DIR}/media_manager/src/audio/from_mic_to_file_recorder_thread.cc
        )
    endif()
    list(APPEND AUDIO_INCLUDES
       ${COMPONENTS_DIR}/media_manager/include/media_manager/audio/from_mic_recorder_adapter.h
       ${COMPONENTS_DIR}/media_manager/include/media_manager/audio/from_mic_to_file_recorder_thread.h
    )
    list(APPEND AUDIO_SOURCES
       ${COMPONENTS_DIR}/media_manager/src/audio/from_mic_recorder_adapter.cc
    )

endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    source_group("Header Files" FILES ${COMMON_INCLUDES})
    source_group("Header Files\\audio" FILES ${AUDIO_INCLUDES})
    source_group("Header Files\\video" FILES ${VIDEO_INCLUDES})

    source_group("Source Files" FILES ${COMMON_SOURCES} )
    source_group("Source Files\\audio" FILES ${AUDIO_SOURCES})
    source_group("Source Files\\video" FILES ${VIDEO_SOURCES})
endif()

add_library("MediaManager"
    ${COMMON_INCLUDES}
    ${AUDIO_INCLUDES}
    ${VIDEO_INCLUDES}
    ${COMMON_SOURCES}
    ${AUDIO_SOURCES}
    ${VIDEO_SOURCES}
)

target_link_libraries("MediaManager" ${LIBRARIES})

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

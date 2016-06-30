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

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/platform.cmake)
include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

include_directories(
  ${COMPONENTS_DIR}/media_manager/include
  ${COMPONENTS_DIR}/utils/include
  ${COMPONENTS_DIR}/protocol_handler/include
  ${COMPONENTS_DIR}/connection_handler/include
  ${COMPONENTS_DIR}/application_manager/include
  ${COMPONENTS_DIR}/smart_objects/include
  ${COMPONENTS_DIR}/hmi_message_handler/include
  ${COMPONENTS_DIR}/formatters/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/policy/include
  ${CMAKE_BINARY_DIR}/src/components
  ${JSONCPP_INCLUDE_DIRECTORY}
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(EXCLUDE_PATHS)

set(LIBRARIES
  protocol
  application_manager
)

if(EXTENDED_MEDIA_MODE)
  find_package(SDLGstreamer REQUIRED)
  include_directories(
    ${GSTREAMER_INCLUDE_DIRECTORY}
  )
  list(APPEND LIBRARIES
    ${GSTREAMER_LIBRARIES}
  )

  get_sdk(SDK)
  if(SDK)
    if(${SDK} STREQUAL "qt")
      # Files copied for the Qt MOC.
      # Because MOC is looking headre nearby with *.cc
      # This headers include in .git exception. see .gitignore
      execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                      ${COMPONENTS_DIR}/media_manager/include/media_manager/audio/from_mic_to_file_recorder_thread.h
                      ${COMPONENTS_DIR}/media_manager/src/audio/from_mic_to_file_recorder_thread.h)
      set(CMAKE_INCLUDE_CURRENT_DIR ON)
      qt5_generate_moc(from_mic_to_file_recorder_thread_qt.cc from_mic_to_file_recorder_thread_qt.moc)

      find_package(Qt5 REQUIRED Multimedia)
      list(APPEND LIBRARIES
        Qt5::Multimedia
      )
      list(APPEND EXCLUDE_PATHS
        from_mic_to_file_recorder_thread.cc
      )
    endif()
  endif()
else()
  list(APPEND EXCLUDE_PATHS
    from_mic_to_file_recorder_thread
    from_mic_recorder_adapter
  )
endif()

collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")
add_library(media_manager ${SOURCES})
target_link_libraries(media_manager ${LIBRARIES})

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

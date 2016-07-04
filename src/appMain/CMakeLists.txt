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

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/sources.cmake)

find_package(SDLOpenSSL REQUIRED)

include_directories(
  ${COMPONENTS_DIR}/protocol_handler/include
  ${COMPONENTS_DIR}/application_manager/include
  ${COMPONENTS_DIR}/formatters/include
  ${COMPONENTS_DIR}/transport_manager/include
  ${COMPONENTS_DIR}/security_manager/include
  ${COMPONENTS_DIR}/security_manager/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/utils/include
  ${COMPONENTS_DIR}/connection_handler/include
  ${COMPONENTS_DIR}/hmi_message_handler/include
  ${COMPONENTS_DIR}/request_watchdog/include
  ${COMPONENTS_DIR}/smart_objects/include
  ${COMPONENTS_DIR}/media_manager/include
  ${COMPONENTS_DIR}/telemetry_monitor/include
  ${COMPONENTS_DIR}/policy/include
  ${COMPONENTS_DIR}/resumption/include
  ${COMPONENTS_DIR}/dbus/include
  ${CMAKE_BINARY_DIR}/src/components
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${LOG4CXX_INCLUDE_DIRECTORY}
  ${OPENSSL_INCLUDE_DIRECTORY}
  ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
)

collect_sources(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}")

set(LIBRARIES
  media_manager
  application_manager
  am_message_helper
  HMI_API
  MOBILE_API
  v4_protocol_v1_2_no_extra
  smart_object
  formatters
  protocol
  protocol_handler
  connection_handler
  transport_manager
  hmi_message_handler
  message_broker
  message_broker_client
  message_broker_server
  utils
  jsoncpp
  config_profile
  resumption
  usage_statistics
)

if(ENABLE_SECURITY)
  list(APPEND LIBRARIES
    security_manager
  )
endif()

if(TELEMETRY_MONITOR)
  list(APPEND LIBRARIES
    telemetry_monitor
  )
endif()

add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES})

add_dependencies(${PROJECT} Policy)

# Replace commit in ini file
set(GITCOMMIT "")
if(EXISTS ${CMAKE_SOURCE_DIR}/.git)
  find_package(Git)
  if(GIT_FOUND)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      OUTPUT_VARIABLE GITCOMMIT ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(INI_FILE ${CMAKE_CURRENT_BINARY_DIR}/smartDeviceLink.ini)
    if(EXISTS ${INI_FILE})
      file(READ ${INI_FILE} FILE_CONTENT)
      set(LINE SDLVersion)
      set(SEARCH_REGEX "${LINE}([^/\r/\n]+)")
      string(REGEX REPLACE "${SEARCH_REGEX}" "${LINE} = ${GITCOMMIT}" 
             MODIFIED_FILE_CONTENT "${FILE_CONTENT}")
      file(WRITE "${INI_FILE}" "${MODIFIED_FILE_CONTENT}")
    endif()
  endif(GIT_FOUND)
endif()

# Install rules
install(TARGETS ${PROJECT}
  DESTINATION ${INSTALL_DIRECTORY}
  PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

install(
  FILES log4cxx.properties audio.8bit.wav test.txt smartDeviceLink.ini
    hmi_capabilities.json sdl_preloaded_pt.json
    ${CMAKE_SOURCE_DIR}/mycert.pem ${CMAKE_SOURCE_DIR}/mykey.pem
  DESTINATION ${INSTALL_DIRECTORY}
)

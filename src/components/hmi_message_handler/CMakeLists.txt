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


include_directories (
    ${COMPONENTS_DIR}/hmi_message_handler/include/
    ${COMPONENTS_DIR}/application_manager/include/
    ${COMPONENTS_DIR}/utils/include/
    ${COMPONENTS_DIR}/smart_objects/include/
    ${COMPONENTS_DIR}/formatters/include/
    ${COMPONENTS_DIR}/config_profile/include/
    ${COMPONENTS_DIR}/media_manager/include/
    ${COMPONENTS_DIR}/protocol_handler/include
    ${JSONCPP_INCLUDE_DIRECTORY}
    ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
    ${CMAKE_BINARY_DIR}/src/components/
    ${COMPONENTS_DIR}/dbus/include/
    ${LOG4CXX_INCLUDE_DIRECTORY}
)

IF (HMIADAPTER STREQUAL "dbus")
    set (DBUS_SOURCE ${COMPONENTS_DIR}/hmi_message_handler/src/dbus_message_adapter.cc)
    set (DBUS_ADAPTER dbus)
ENDIF ()

set (SOURCES
    ${COMPONENTS_DIR}/hmi_message_handler/src/hmi_message_handler_impl.cc
    ${COMPONENTS_DIR}/hmi_message_handler/src/messagebroker_adapter.cc
    ${COMPONENTS_DIR}/hmi_message_handler/src/hmi_message_adapter_impl.cc
    ${DBUS_SOURCE}
)
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(LIBRARIES
    Utils
    ${DBUS_ADAPTER}
    ${RTLIB}
)
else()
  set(LIBRARIES   Utils )
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set (HEADERS
        ${COMPONENTS_DIR}/include/hmi_message_handler/hmi_message_adapter.h
        ${COMPONENTS_DIR}/hmi_message_handler/include/hmi_message_handler/hmi_message_handler_impl.h
        ${COMPONENTS_DIR}/hmi_message_handler/include/hmi_message_handler/messagebroker_adapter.h
        ${DBUS_HEADER}
  )
  source_group("Header Files" FILES ${HEADERS} )
  source_group("Source Files" FILES ${SOURCES} )
  add_library("HMIMessageHandler" ${HEADERS} ${SOURCES})
else()
  list(APPEND SOURCES
    ${COMPONENTS_DIR}/hmi_message_handler/src/mqueue_adapter.cc
  )
  add_library("HMIMessageHandler" ${SOURCES})
  if(ENABLE_LOG)
      target_link_libraries("HMIMessageHandler" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
  endif()
endif()

target_link_libraries("HMIMessageHandler" ${LIBRARIES})

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

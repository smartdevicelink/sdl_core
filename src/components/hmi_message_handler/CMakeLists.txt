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

include_directories (
    include/
    ${COMPONENTS_DIR}/application_manager/include/
    ${COMPONENTS_DIR}/utils/include/
    ${COMPONENTS_DIR}/smart_objects/include/
    ${COMPONENTS_DIR}/formatters/include/
    ${COMPONENTS_DIR}/media_manager/include/
    ${COMPONENTS_DIR}/protocol_handler/include
    ${JSONCPP_INCLUDE_DIRECTORY}
    ${MESSAGE_BROKER_INCLUDE_DIRECTORY}
    ${COMPONENTS_DIR}
    ${CMAKE_BINARY_DIR}/src/components/
    ${COMPONENTS_DIR}/dbus/include/
    ${CMAKE_SOURCE_DIR}/
    ${LOG4CXX_INCLUDE_DIRECTORY}
    ${COMPONENTS_DIR}/hmi_message_handler/include
    ${COMPONENTS_DIR}/hmi_message_handler/src
    ${BOOST_INCLUDE_DIR}
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (HMIADAPTER STREQUAL "dbus")
  set(EXCLUDE_PATHS messagebroker_adapter.cc)
  set(DBUS_ADAPTER DBus)
else()
  set(EXCLUDE_PATHS dbus_message_adapter.cc)
  set(DBUS_ADAPTER)
endif ()

list(APPEND EXCLUDE_PATHS
  mqueue_adapter.cc
)

collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")

set(LIBRARIES
  Utils
  ${DBUS_ADAPTER}
  ${RTLIB}
)

add_library("HMIMessageHandler" ${SOURCES})

if (HMIADAPTER STREQUAL "messagebroker")
  add_dependencies("HMIMessageHandler" Boost)
endif()

target_link_libraries("HMIMessageHandler" ${LIBRARIES})

if(ENABLE_LOG)
  target_link_libraries("HMIMessageHandler" log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
endif()

# Tests temporary are inactivated. For details please check
# Readme.txt in test directory of hmi_message_handler 

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

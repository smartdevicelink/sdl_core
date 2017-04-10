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

set(MESSAGE_BROKER_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(MESSAGE_BROKER_INCLUDE_DIRECTORY ${MESSAGE_BROKER_INCLUDE_DIRECTORY} PARENT_SCOPE)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${JSONCPP_INCLUDE_DIRECTORY}
)

set(CLIENT_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/client
)
collect_sources(CLIENT_SOURCES "${CLIENT_PATHS}")

set(SERVER_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/server
)
collect_sources(SERVER_SOURCES "${SERVER_PATHS}")

set(EXCLUDE_PATHS
  ${CLIENT_PATHS}
  ${SERVER_PATHS}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/example
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
collect_sources(SOURCES "${PATHS}" "${EXCLUDE_PATHS}")

set(LIBRARIES
  Utils
)

add_library(message_broker ${SOURCES})
target_link_libraries(message_broker ${LIBRARIES})

list(APPEND LIBRARIES
  message_broker
)

add_library(message_broker_client ${CLIENT_SOURCES})
target_link_libraries(message_broker_client ${LIBRARIES})

add_library(message_broker_server ${SERVER_SOURCES})
target_link_libraries(message_broker_server ${LIBRARIES})

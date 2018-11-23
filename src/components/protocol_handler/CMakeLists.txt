# Copyright (c) 2018, Ford Motor Company All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

project(protocol_handler)

add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(
  ${PROJECT_NAME}
  PRIVATE
  "$<$<BOOL:${ENABLE_SECURITY}>:${CMAKE_CURRENT_LIST_DIR}/src/handshake_handler.cc>"
  "${CMAKE_CURRENT_LIST_DIR}/src/incoming_data_handler.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/multiframe_builder.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/protocol_handler_impl.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/protocol_packet.cc"
  "${CMAKE_CURRENT_LIST_DIR}/src/protocol_payload.cc")

sdl_find_package(BSON REQUIRED)

target_link_libraries(${PROJECT_NAME}
                      PUBLIC config_profile::config_profile
                             protocol::protocol
                             utils::utils
                             connection_handler::connection_handler
                      PRIVATE components::interface BSON::BSON)

if(TELEMETRY_MONITOR)
  target_link_libraries(${PROJECT_NAME}
                        PUBLIC telemetry_monitor::telemetry_monitor)
endif()

target_include_directories(${PROJECT_NAME}
                           PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

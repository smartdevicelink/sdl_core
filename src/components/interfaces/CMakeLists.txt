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

project(interfaces)

include("${CMAKE_SOURCE_DIR}/tools/cmake/helpers/generators.cmake")

set(TARGET_NAME v4_protocol_v1_2_no_extra)
add_library(${TARGET_NAME} STATIC)
add_library(${PROJECT_NAME}::${TARGET_NAME} ALIAS ${TARGET_NAME})
generate_interface("${CMAKE_CURRENT_SOURCE_DIR}/v4_protocol_v1_2_no_extra.xml" "NsSmartDeviceLinkRPC::V1" "sdlrpcv1" ${TARGET_NAME})
target_link_libraries(${TARGET_NAME}
  PUBLIC
    smart_objects::smart_objects
    formatters::formatters
  PRIVATE
    components::interface
)

set(TARGET_NAME MOBILE_API)
add_library(${TARGET_NAME} STATIC)
add_library(${PROJECT_NAME}::${TARGET_NAME} ALIAS ${TARGET_NAME})
generate_interface("${CMAKE_CURRENT_SOURCE_DIR}/MOBILE_API.xml" "mobile_apis" "sdlrpcv2" ${TARGET_NAME})
target_link_libraries(${TARGET_NAME}
  PUBLIC
    smart_objects::smart_objects
    formatters::formatters
  PRIVATE
    components::interface
)

if(HMI_JSON_API)
  set(TARGET_NAME HMI_API)
  add_library(${TARGET_NAME} STATIC)
  add_library(${PROJECT_NAME}::${TARGET_NAME} ALIAS ${TARGET_NAME})
  generate_interface("${CMAKE_CURRENT_SOURCE_DIR}/HMI_API.xml" "hmi_apis" "jsonrpc" ${TARGET_NAME})
  target_link_libraries(${TARGET_NAME}
    PUBLIC
      smart_objects::smart_objects
      formatters::formatters
    PRIVATE
      components::interface
  )
endif()

if(HMI_DBUS_API)
  set(TARGET_NAME HMI_API)
  add_library(${TARGET_NAME} STATIC)
  add_library(${PROJECT_NAME}::${TARGET_NAME} ALIAS ${TARGET_NAME})
  generate_interface("${CMAKE_CURRENT_SOURCE_DIR}/QT_HMI_API.xml" "hmi_apis" "jsonrpc" ${TARGET_NAME})
  target_link_libraries(${TARGET_NAME}
    PUBLIC
      smart_objects::smart_objects
      formatters::formatters
    PRIVATE
      components::interface
  )
endif()
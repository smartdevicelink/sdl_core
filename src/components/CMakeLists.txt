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

set(COMPONENTS_DIR ${CMAKE_CURRENT_LIST_DIR})
project(components)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::interface ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                           INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")

if("${EXTENDED_POLICY}" STREQUAL "EXTERNAL_PROPRIETARY")
  target_include_directories(
    ${PROJECT_NAME}
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include/policy/policy_external")
  target_compile_definitions(${PROJECT_NAME} INTERFACE
                             EXTERNAL_PROPRIETARY_MODE)
  set(POLICY_MOCK_INCLUDE_PATH
      ${COMPONENTS_DIR}/include/test/policy/policy_external/)
  add_subdirectory(policy/policy_external)
else()
  if("${EXTENDED_POLICY}" STREQUAL "PROPRIETARY")
    target_compile_definitions(${PROJECT_NAME} INTERFACE PROPRIETARY_MODE)
  endif()
  target_include_directories(
    ${PROJECT_NAME}
    INTERFACE "${COMPONENTS_DIR}/include/policy/policy_regular")
  add_subdirectory(policy/policy_regular)
  set(POLICY_MOCK_INCLUDE_PATH
      ${COMPONENTS_DIR}/include/test/policy/policy_regular/)
endif()

# Just in case
if(ENABLE_LOG)
  target_compile_definitions(${PROJECT_NAME} INTERFACE ENABLE_LOG)
endif()

add_subdirectory(application_manager)
add_subdirectory(config_profile)
add_subdirectory(connection_handler)
add_subdirectory(formatters)
add_subdirectory(hmi_message_handler)
add_subdirectory(interfaces)
add_subdirectory(media_manager)
add_subdirectory(protocol)
add_subdirectory(protocol_handler)
add_subdirectory(resumption)
add_subdirectory(rpc_base)
add_subdirectory(smart_objects)
add_subdirectory(transport_manager)
add_subdirectory(utils)

if(ENABLE_SECURITY)
  add_subdirectory(security_manager)
endif()

if(TELEMETRY_MONITOR)
  add_subdirectory(telemetry_monitor)
endif()

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

include_directories (
  include/
  ${COMPONENTS_DIR}/include/application_manager/
  ${COMPONENTS_DIR}/telemetry_monitor/include/
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/protocol_handler/include/
  ${COMPONENTS_DIR}/connection_handler/include/
  ${COMPONENTS_DIR}/transport_manager/include/
  ${COMPONENTS_DIR}/application_manager/include/
  ${COMPONENTS_DIR}/can_cooperation/include/
  ${POLICY_PATH}/include/
  ${POLICY_GLOBAL_INCLUDE_PATH}/
  ${COMPONENTS_DIR}/rpc_base/include/
  ${COMPONENTS_DIR}/functional_module/include/
  ${COMPONENTS_DIR}/hmi_message_handler/include/
  ${COMPONENTS_DIR}/formatters/include/
  ${COMPONENTS_DIR}/media_manager/include/
  ${COMPONENTS_DIR}/smart_objects/include/
  ${COMPONENTS_DIR}/config_profile/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_BINARY_DIR}/src/components/
  ${LOG4CXX_INCLUDE_DIRECTORY}
)

set(PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
collect_sources(SOURCES "${PATHS}")

set(LIBRARIES
  HMI_API
  MOBILE_API
  Utils
  Policy
)

add_library("TelemetryMonitor" ${SOURCES})
target_link_libraries("TelemetryMonitor" ${LIBRARIES})
add_dependencies("TelemetryMonitor" HMI_API MOBILE_API)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

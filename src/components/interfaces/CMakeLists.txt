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

include(${CMAKE_SOURCE_DIR}/tools/cmake/helpers/generators.cmake)

include_directories (
  ${COMPONENTS_DIR}/include
  ${COMPONENTS_DIR}/utils/include
  ${COMPONENTS_DIR}/formatters/include
  ${COMPONENTS_DIR}/smart_objects/include
  ${CMAKE_BINARY_DIR}
)

generate_interface("v4_protocol_v1_2_no_extra.xml" "NsSmartDeviceLinkRPC::V1" "sdlrpcv1")
add_library(v4_protocol_v1_2_no_extra ${CMAKE_CURRENT_BINARY_DIR}/v4_protocol_v1_2_no_extra_schema.cc)

generate_interface("MOBILE_API.xml" "mobile_apis" "sdlrpcv2")
add_library(MOBILE_API ${CMAKE_CURRENT_BINARY_DIR}/MOBILE_API_schema.cc)

generate_interface("HMI_API.xml" "hmi_apis" "jsonrpc")
add_library(HMI_API ${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.cc)

target_link_libraries(v4_protocol_v1_2_no_extra Utils)
target_link_libraries(MOBILE_API Utils)
target_link_libraries(HMI_API Utils)


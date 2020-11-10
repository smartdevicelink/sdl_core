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

include_directories(
  include
  ${COMPONENTS_DIR}
  ${GMOCK_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${POLICY_PATH}/include
  ${COMPONENTS_DIR}/rpc_base/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/include/test/policy/policy_regular/
  ${POLICY_PATH}/test/include/
  ${POLICY_MOCK_INCLUDE_PATH}/
)

file (GLOB POLICY_TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/cache_manager_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/policy_manager_impl_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/access_remote_impl_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/sql_pt_representation_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/update_status_manager_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/vehicle_data_item_type_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/shared_library_test.cc
)

set(LIBRARIES
  gmock
  Utils
  PolicyStatic
  UsageStatistics
)

create_test(policy_test "${POLICY_TEST_SOURCES}" "${LIBRARIES}")

#Copy actual sdl_preloaded_pt.json
file(COPY ${CMAKE_SOURCE_DIR}/src/appMain/sdl_preloaded_pt.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/json/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/json/sdl_preloaded_pt_for_merge_initial.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/json/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/json/sdl_preloaded_pt_for_merge_latest.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/json/)

# Copyright (c) 2015, Ford Motor Company
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

include_directories(
  include
  ${GMOCK_INCLUDE_DIRECTORY}
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${POLICY_PATH}/include
  ${COMPONENTS_DIR}/rpc_base/include
  ${COMPONENTS_DIR}/config_profile/include
  ${COMPONENTS_DIR}/utils/include/
  ${POLICY_MOCK_INCLUDE_PATH}
)

set(testLibraries
  gmock
  Utils
  Policy
  UsageStatistics
  ConfigProfile
)

file (GLOB POLICY_TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/policy_manager_impl_test_base.cc
  *.cc)

# Unbuildable
list (REMOVE_ITEM POLICY_TEST_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/policy_manager_impl_stress_test.cc)

create_test("policy_test" "${POLICY_TEST_SOURCES}" "${testLibraries}")

#Additional test files (json)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/json)
file(GLOB JSON_TEST_FILES json/*.json)
file(COPY ${JSON_TEST_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/json)

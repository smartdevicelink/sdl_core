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


if(BUILD_TESTS)

include_directories(
  ${GMOCK_INCLUDE_DIRECTORY}
  ${COMPONENTS_DIR}/smart_objects/include
  ${COMPONENTS_DIR}/formatters/include
  ${COMPONENTS_DIR}/formatters/test/include
  ${CMAKE_BINARY_DIR}/src/components/interfaces
  ${CMAKE_SOURCE_DIR}/src/3rd_party-static/jsoncpp/include
)

set(LIBRARIES
  gmock 
  HMI_API
  MOBILE_API
  SmartObjects
  formatters
  jsoncpp
)

set(SOURCES
  ${COMPONENTS_DIR}/formatters/test/src/SmartFactoryTestHelper.cc
  ${COMPONENTS_DIR}/formatters/test/CSmartFactory_test.cc
  ${COMPONENTS_DIR}/formatters/test/CFormatterJsonBase_test.cc
  ${COMPONENTS_DIR}/formatters/test/generic_json_formatter_test.cc
  ${COMPONENTS_DIR}/formatters/test/formatter_json_rpc_test.cc
  ${COMPONENTS_DIR}/formatters/test/src/create_smartSchema.cc 
  ${COMPONENTS_DIR}/formatters/test/cFormatterJsonSDLRPCv1_test.cc
  ${COMPONENTS_DIR}/formatters/test/cFormatterJsonSDLRPCv2_test.cc
  ${COMPONENTS_DIR}/formatters/test/src/meta_formatter_test_helper.cc
  ${COMPONENTS_DIR}/formatters/test/meta_formatter_test.cc
)

create_test("formatters_test" "${SOURCES}" "${LIBRARIES}")

endif()

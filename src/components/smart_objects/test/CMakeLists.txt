# Copyright (c) 2014, Ford Motor Company
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
add_definitions(-DUNIT_TESTS)

include_directories (
  ${GMOCK_INCLUDE_DIRECTORY}
  ${COMPONENTS_DIR}/smart_objects/include
  ${COMPONENTS_DIR}/formatters/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
)

set(LIBRARIES
    gmock_main
    SmartObjects
    formatters
    jsoncpp   
)

set(SOURCES
  ${COMPONENTS_DIR}/smart_objects/test/SmartObjectDraft_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/SmartObjectInvalid_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/SmartObjectStress_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/SmartObjectUnit_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/smart_object_performance_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/map_performance_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/BoolSchemaItem_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/NumberSchemaItem_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/StringSchemaItem_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/ArraySchemaItem_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/CObjectSchemaItem_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/AlwaysTrueSchemaItem_test.cc
  ${COMPONENTS_DIR}/smart_objects/test/AlwaysFalseSchemaItem_test.cc
)

create_test("smart_object_test" "${SOURCES}" "${LIBRARIES}")

endif()

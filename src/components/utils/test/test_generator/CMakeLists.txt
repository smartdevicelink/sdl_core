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
  ${CMAKE_SOURCE_DIR}/tools/interfaceGenerator
  ${CMAKE_CURRENT_BINARY_DIR}
  ${COMPONENTS_DIR}/utils/include/utils
  ${COMPONENTS_DIR}/include/utils
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${GMOCK_INCLUDE_DIRECTORY}
)

set(XML_NAME ${CMAKE_SOURCE_DIR}/src/components/interfaces/MOBILE_API.xml)
add_custom_target(generate_version
  COMMAND ${INTEFRACE_GENERATOR_CMD} ${XML_NAME} "mobile_apis"
  ${CMAKE_CURRENT_BINARY_DIR} "--parser-type" "sdlrpcv2"
  DEPENDS ${INTERFACE_GENERATOR_DEPENDENCIES} ${XML_NAME}
  VERBATIM
)

set(LIBRARIES
  gmock
  Utils
)

set(SOURCES
  generated_msg_version_test.cc
)

create_test(generator_test "${SOURCES}" "${LIBRARIES}")
file(COPY ${XML_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_dependencies("generator_test" generate_version)

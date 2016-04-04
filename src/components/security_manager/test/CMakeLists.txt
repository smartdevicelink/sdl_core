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

if (BUILD_TESTS)
  
include_directories(
  ${GMOCK_INCLUDE_DIRECTORY}
  ${COMPONENTS_DIR}/protocol_handler/test/include
  ${COMPONENTS_DIR}/transport_manager/test/include
  ${COMPONENTS_DIR}/security_manager/include
  ${COMPONENTS_DIR}/resumption/include
  ${JSONCPP_INCLUDE_DIRECTORY}
)

set(SOURCES
  ${COMPONENTS_DIR}/security_manager/test/crypto_manager_impl_test.cc
  ${COMPONENTS_DIR}/security_manager/test/security_manager_test.cc
  ${COMPONENTS_DIR}/security_manager/test/security_query_test.cc
  ${COMPONENTS_DIR}/security_manager/test/security_query_matcher.cc
  ${COMPONENTS_DIR}/security_manager/test/ssl_context_test.cc
  ${COMPONENTS_DIR}/security_manager/test/ssl_certificate_handshake_test.cc
 )

set(LIBRARIES
  gmock
  ${SecurityManagerLibrary}
)

set(CERT_LIST 
  ${CMAKE_SOURCE_DIR}/mycert.pem
  ${CMAKE_SOURCE_DIR}/mykey.pem   
)

add_custom_target(generate_certificates
  COMMAND ${CMAKE_SOURCE_DIR}/tools/Utils/generate_test_certificates.py
          -d=${CMAKE_CURRENT_BINARY_DIR} -s
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

foreach( file_i ${CERT_LIST})
  file(COPY ${file_i} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach( file_i )

create_test (security_manager_test "${SOURCES}" "${LIBRARIES}")
add_dependencies(security_manager_test generate_certificates)

endif ()

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

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  find_package(WinOpenSSL REQUIRED)
endif()

include_directories(
  include/
  ${COMPONENTS_DIR}/protocol_handler/include/
  ${COMPONENTS_DIR}/connection_handler/include
  ${COMPONENTS_DIR}/config_profile/include/
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/application_manager/include/
  ${JSONCPP_INCLUDE_DIRECTORY}
  ${CMAKE_SOURCE_DIR}/src/thirdPartyLibs/jsoncpp/include
  ${APR_INCLUDE_DIRECTORY}
  ${OPENSSL_INCLUDE_DIRECTORY}
)

set (SOURCES
  ${COMPONENTS_DIR}/security_manager/src/security_manager_impl.cc
  ${COMPONENTS_DIR}/security_manager/src/security_query.cc
  ${COMPONENTS_DIR}/security_manager/src/crypto_manager_impl.cc
  ${COMPONENTS_DIR}/security_manager/src/ssl_context_impl.cc
)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    file (GLOB inc ${COMPONENTS_DIR}/security_manager/include/security_manager/*.h)
    string( REPLACE / \\ group ${inc} )
    source_group("Header Files" FILES ${inc} )
    add_library("SecurityManager" ${inc} ${SOURCES})
    target_link_libraries(SecurityManager libeay32 ssleay32 ProtocolHandler jsoncpp ProtocolLibrary)
else()
  add_library(SecurityManager ${SOURCES})
  target_link_libraries(SecurityManager crypto ssl ProtocolHandler jsoncpp ProtocolLibrary)
endif()


if(BUILD_TESTS)
  #add_subdirectory(test)
endif()

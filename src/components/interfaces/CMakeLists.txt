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


include_directories (
  ${COMPONENTS_DIR}/utils/include/
  ${COMPONENTS_DIR}/formatters/include
)
  
GenerateInterface("v4_protocol_v1_2_no_extra.xml" "NsSmartDeviceLinkRPC::V1" "sdlrpcv1")
GenerateInterface("MOBILE_API.xml" "mobile_apis" "sdlrpcv2")

IF (${HMI_JSON_API})
  GenerateInterface("HMI_API.xml" "hmi_apis" "jsonrpc")
ENDIF (${HMI_JSON_API})

IF (${HMI_DBUS_API})
    set(hpp_file
      "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API.h"
      "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.h"
    )

    set(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.cc")
    set(full_xml_name "${CMAKE_CURRENT_SOURCE_DIR}/QT_HMI_API.xml")

    add_custom_command( OUTPUT ${hpp_file} ${cpp_file}
                        COMMAND ${INTEFRACE_GENERATOR_CMD} ${full_xml_name} hmi_apis ${CMAKE_CURRENT_BINARY_DIR} "--parser-type" "jsonrpc"
                        DEPENDS ${INTERFACE_GENERATOR_DEPENDENCIES} ${full_xml_name}
      COMMENT "Generating files:\n   ${hpp_file}\n   ${cpp_file}\nfrom:\n   QT_HMI_API.xml ..."
                        VERBATIM
                       )

    include_directories (
        ${COMPONENTS_DIR}/smart_objects/include
        ${COMPONENTS_DIR}/formatters/include/
        ${CMAKE_BINARY_DIR}
    )

    add_custom_command(
          OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/HMI_API.h"
    COMMAND sed "-e" "s/QT_HMI_API/HMI_API/g" "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API.h" > "${CMAKE_CURRENT_BINARY_DIR}/HMI_API.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API.h")
    add_custom_command(
          OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.h"
    COMMAND sed "-e" "s/QT_HMI_API/HMI_API/g" "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.h" > "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.h")
    add_custom_command(
          OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.cc"
    COMMAND sed "-e" "s/QT_HMI_API/HMI_API/g" "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.cc" > "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.cc"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/QT_HMI_API_schema.cc" "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.h" "${CMAKE_CURRENT_BINARY_DIR}/HMI_API.h")
  add_library(HMI_API "${CMAKE_CURRENT_BINARY_DIR}/HMI_API_schema.cc")
ENDIF (${HMI_DBUS_API})

add_dependencies(HMI_API Utils)

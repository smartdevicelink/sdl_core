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
  ${DBUS_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
  ${CMAKE_SOURCE_DIR}/
  ${COMPONENTS_DIR}
  ${COMPONENTS_DIR}/formatters/include/
  ${COMPONENTS_DIR}/smart_objects/include/
  ${COMPONENTS_DIR}/utils/include/
  ${CMAKE_BINARY_DIR}/src/components/
)

set (SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/introspection_xml.cc
  ${CMAKE_CURRENT_BINARY_DIR}/message_descriptions.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/dbus_adapter.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/dbus_message.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/dbus_message_controller.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/schema.cc
)

set (LIBRARIES dbus-1 -L${DBUS_LIBS_DIRECTORY})


add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/message_descriptions.cc
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/codegen/make_message_descriptions.py
  --infile ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/QT_HMI_API.xml
  --outdir ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/QT_HMI_API.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/codegen/make_message_descriptions.py
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/introspection_xml.cc
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/codegen/make_introspection_c.py
  --infile ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/QT_HMI_API.xml
  --outdir ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/QT_HMI_API.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/codegen/make_introspection_c.py
)

add_library("DBus" ${SOURCES})
ADD_DEPENDENCIES("DBus" Utils install-3rd_party_dbus)

target_link_libraries("DBus" "${LIBRARIES}")

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

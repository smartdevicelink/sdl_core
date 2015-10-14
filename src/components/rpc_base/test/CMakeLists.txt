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

if (BUILD_TESTS)
  include_directories (
    ${COMPONENTS_DIR}/dbus/include
    ${COMPONENTS_DIR}/dbus/src
    ${COMPONENTS_DIR}/rpc_base/include
    ${CMAKE_SOURCE_DIR}/src/3rd_party/dbus-1.7.8
    ${GMOCK_INCLUDE_DIRECTORY}
    ${JSONCPP_INCLUDE_DIRECTORY}
)

set(LIBRARIES
  gmock
  jsoncpp
)

set(SOURCES
  rpc_base_json_test.cc
  rpc_base_test.cc
  validation_report_test.cc
)

if (${HMI_DBUS_API})
  # Build dbus tests
  include_directories(${DBUS_INCLUDE_DIRS})
  set (LIBRARIES ${LIBRARIES} DBus)
  set (SOURCES ${SOURCES} rpc_base_dbus_test.cc)
endif ()

create_test("rpc_base_test" "${SOURCES}" "${LIBRARIES}")

endif()


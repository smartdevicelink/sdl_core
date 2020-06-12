# Copyright (c) 2019, Ford Motor Company, Livio
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
# Neither the name of the copyright holders nor the names of their contributors
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


set(APP_SERVICE_DIR "${COMPONENTS_DIR}/application_manager/rpc_plugins/app_service_rpc_plugin")
set(APP_SERVICE_SRC_DIR "${APP_SERVICE_DIR}/src")
set(APP_SERVICE_INCLUDE_DIR "${APP_SERVICE_DIR}/include/app_service_rpc_plugin")
set(APP_SERVICE_TEST_DIR "${APP_SERVICE_DIR}/test")

include_directories(
  ${GMOCK_INCLUDE_DIRECTORY}
  ${APP_SERVICE_INCLUDE_DIR}
) 

set(SOURCES ${APP_SERVICE_TEST_DIR}/app_service_plugin/app_service_app_extension_test.cc
            ${COMPONENTS_DIR}/application_manager/test/mock_message_helper.cc
)

set(LIBRARIES
  gmock
  sdl_rpc_plugin_static
  app_service_rpc_plugin_static
)

create_test("app_service_plugin_test" "${SOURCES}" "${LIBRARIES}" )

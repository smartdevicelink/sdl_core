# Copyright (c) 2019, Livio
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:

# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.

# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.

# Neither the name of the copyright holders nor the names of their contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.

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

include_directories (
  ${GMOCK_INCLUDE_DIRECTORY}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${COMPONENTS_DIR}/application_manager/test/include
  ${COMPONENTS_DIR}/include/test/application_manager/
  ${COMPONENTS_DIR}/resumption/include
  )

set(APP_SERVICE_COMMANDS_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/commands)

file(GLOB SOURCES
  ${COMPONENTS_DIR}/application_manager/test/mock_message_helper.cc
  ${APP_SERVICE_COMMANDS_TEST_DIR}/*
  ${APP_SERVICE_COMMANDS_TEST_DIR}/mobile/*
)

set(LIBRARIES
  AppServiceRpcPluginStaticLib
  ApplicationManager
  connectionHandler
  SmartObjects
  ProtocolHandler
  MessageHelper
  connectionHandler
  Utils
  jsoncpp
  gmock_main
  dl
  Resumption
)

if(ENABLE_LOG)
  list(APPEND LIBRARIES log4cxx -L${LOG4CXX_LIBS_DIRECTORY})
  list(APPEND LIBRARIES apr-1 -L${APR_LIBS_DIRECTORY})
  list(APPEND LIBRARIES aprutil-1 -L${APR_UTIL_LIBS_DIRECTORY})
  list(APPEND LIBRARIES expat -L${EXPAT_LIBS_DIRECTORY})
endif()

create_test("app_service_commands_test" "${SOURCES}" "${LIBRARIES}" )

